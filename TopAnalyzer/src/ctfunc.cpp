#include "ctfunc.h"
#include "featureset.h"

#include <queue>
#include <cstring>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>

template <typename T>
static inline T clamp(T x, T a, T b)
{
  return x < a ? a : (x > b ? b : x);
}

size_t count_branches(ctBranch* b)
{
  if(b == NULL) return 0;

  size_t count = 0;
  FeatureSet* fs = (FeatureSet*) b->data;
  if(fs == NULL || fs->remove == false) count = 1;

  for(ctBranch*c = b->children.head; c != NULL; c = c->nextChild)
    count += count_branches(c);

  return count;
}

size_t calc_persistence_branch(ctBranch* b, top::Dataset* data)
{
  return std::abs((long) (data->data->Get(b->extremum) - data->data->Get(b->saddle)));
}

void calc_branch_depth(ctBranch* b, size_t* max_depth, size_t depth)
{
  if(b == NULL) return;

  if(b->data == NULL)
    b->data = (FeatureSet*) calloc(1, sizeof(FeatureSet));

  FeatureSet* branch_data = (FeatureSet*) b->data;
  if(!branch_data->remove) {
    branch_data->depth = depth;
    if(depth > *max_depth)
      *max_depth = depth;
  }

  for(ctBranch* c = b->children.head; c != NULL; c = c->nextChild)
    calc_branch_depth(c, max_depth, depth + 1);
}

void calc_branch_features(ctBranch** b_map, top::Dataset* data)
{
  if(b_map == NULL) return;

  for(size_t i = 0; i < data->size; i++) {
    if(b_map[i]->data == NULL)
      b_map[i]->data = calloc(1, sizeof(FeatureSet));

    FeatureSet* branch_data = (FeatureSet*) b_map[i]->data;
    branch_data->v++;
    branch_data->hv += data->data->Get(i);
    branch_data->p = calc_persistence_branch(b_map[i], data);
    branch_data->porosity = 300.0;

    branch_data->c_s_min = 10000.0; //since maximum value is 255
    branch_data->c_s_max = 0;

    if(branch_data->min_intensity > data->data->Get(i))
      branch_data->min_intensity = data->data->Get(i);
    else if(branch_data->max_intensity < data->data->Get(i))
      branch_data->max_intensity = data->data->Get(i);
  }
}

void calc_branch_num_children(ctBranch* root_branch)
{
  if(root_branch == NULL) return;

  if(root_branch->data == NULL)
    root_branch->data = (FeatureSet*) calloc(1, sizeof(FeatureSet));

  std::queue<ctBranch*> branch_queue;
  branch_queue.push(root_branch);

  do {
    ctBranch* curr_branch = branch_queue.front();
    branch_queue.pop();

    if(curr_branch->data == NULL)
      curr_branch->data = (FeatureSet*) calloc(1, sizeof(FeatureSet));

    FeatureSet* branch_data = (FeatureSet*) curr_branch->data;
    for(ctBranch* c = curr_branch->children.head; c != NULL; c = c->nextChild) {
      FeatureSet* c_data = (FeatureSet*) c->data;
      if(!c_data->remove) {
        branch_data->num_children++;
        branch_queue.push(c);
      }
    }
  } while(!branch_queue.empty());
}

FeatureSet* find_max_features_children(ctBranch* root_branch)
{
  FeatureSet* max_features = (FeatureSet*) calloc(1, sizeof(FeatureSet));
  if(root_branch == NULL) return max_features;

  for(ctBranch* c = root_branch->children.head; c != NULL; c = c->nextChild) {
    FeatureSet* c_data = (FeatureSet*) c->data;
    if(c_data->remove) continue;

    if(c_data->v > max_features->v)
      max_features->v = c_data->v;

    if(c_data->hv > max_features->hv)
      max_features->hv = c_data->hv;

    if(c_data->p > max_features->p)
      max_features->p = c_data->p;
  }

  return max_features;
}

void normalize_features(ctBranch* root_branch)
{
  if(root_branch == NULL) return;

  std::queue<ctBranch*> branch_queue;
  branch_queue.push(root_branch);

  FeatureSet* root_data = (FeatureSet*) root_branch->data;
  root_data->norm_v = root_data->norm_p = root_data->norm_hv = 1.0;

  do {
    ctBranch* curr_branch = branch_queue.front();
    branch_queue.pop();

    FeatureSet* max_features = find_max_features_children(curr_branch);

    for(ctBranch* c = curr_branch->children.head; c != NULL; c = c->nextChild) {
      FeatureSet* c_data = (FeatureSet*) c->data;
      if(!c_data->remove) {
        branch_queue.push(c);
        c_data->norm_v = max_features->v != 0? (double) c_data->v / max_features->v : 0;
        c_data->norm_p = max_features->p != 0? (double) c_data->p / max_features->p : 0;
        c_data->norm_hv = max_features->hv != 0? (double) c_data->hv / max_features->hv : 0;

        //std::cout << "    " << c_data->norm_v << ", " << c_data->norm_hv << ", " << c_data->norm_p << std::endl;
      }
    }

    free(max_features);
    max_features = NULL;

  } while(!branch_queue.empty());

}

int label_branches(ctBranch* root_branch)
{
  if(root_branch == NULL) return -1;

  std::queue<ctBranch*> branch_queue;
  branch_queue.push(root_branch);

  int curr_label = 0;
  do {
    ctBranch* curr_branch = branch_queue.front();
    branch_queue.pop();

    FeatureSet* branch_data = (FeatureSet*) curr_branch->data;
    branch_data->label = curr_label++;

    for(ctBranch* c = curr_branch->children.head; c != NULL; c = c->nextChild) {
      FeatureSet* c_data = (FeatureSet*) c->data;
      if(!c_data->remove)
        branch_queue.push(c);
    }

  } while(!branch_queue.empty());

  return curr_label;
}

void zero_branches(ctBranch *root_branch)
{
  if(root_branch == NULL) return;

  if(root_branch->data == NULL)
    root_branch->data = calloc(1, sizeof(FeatureSet));

  FeatureSet* d = (FeatureSet*) root_branch->data;
  memset(d, 0, sizeof(FeatureSet));
  d->min_intensity = 4096;

  for(ctBranch* c = root_branch->children.head; c != NULL; c = c->nextChild)
    zero_branches(c);
}

double calc_avg_importance(ctBranch* root_branch, double (*importance_measure)(ctBranch*))
{
  if(root_branch == NULL) return 0.0;

  double avg_importance = 0.0;
  std::queue<ctBranch*> branch_queue;
  branch_queue.push(root_branch);

  do {
    ctBranch* curr_branch = branch_queue.front();
    branch_queue.pop();

    double branch_importance = importance_measure(curr_branch);
    avg_importance += branch_importance;

    for(ctBranch* c = curr_branch->children.head; c != NULL; c = c->nextChild) {
      branch_queue.push(c);
    }
  } while(!branch_queue.empty());

  return avg_importance / count_branches(root_branch);
}

double std_avg_importance(ctBranch* b)
{
  if(b == NULL) return 0.0;

  FeatureSet* ptr = (FeatureSet*) b->data;
  if(ptr == NULL) return 0.0;

  return 0.5 * sqrt(pow(ptr->hv * ptr->p, 2) + pow(ptr->v * ptr->p, 2) + pow(ptr->hv * ptr->v, 2));
}

double std_log_importance(ctBranch* b)
{
  if(b == NULL) return 0.0;

  FeatureSet* ptr = (FeatureSet*) b->data;
  if(ptr == NULL) return 0.0;

  double a = sqrt(pow(ptr->hv * ptr->p, 2) + pow(ptr->v * ptr->p, 2) + pow(ptr->hv * ptr->v, 2));
  if(a == 0) return 0.0;

  double c = log(a);
  return c;
}

double std_value(size_t v, void* d)
{
  top::Mesh* mesh = reinterpret_cast<top::Mesh*>(d);
  return mesh->data.Get(v);
}

size_t std_neighbors(size_t v, size_t* nbrs, void* d)
{
  top::Mesh* mesh = static_cast<top::Mesh*>(d);
  size_t nbrs_size = mesh->getNeighbors(v, nbrs, 18);
  return nbrs_size;
}

void vertex_proc(size_t v, ctArc* a, void* cb_data)
{
  if(a->data == NULL)
    a->data = (FeatureSet*) calloc(1, sizeof(FeatureSet));//FeatureSet_new();

  top::Mesh* mesh_ptr = (top::Mesh*) cb_data;
  FeatureSet* data_a = (FeatureSet*) a->data;
  data_a->v += 1;
  data_a->hv += mesh_ptr->data.Get(v);
  data_a->p = std::abs((long) (mesh_ptr->data.Get(a->hi->i) - mesh_ptr->data.Get(a->lo->i)));
}

void arc_merge_proc(ctArc* a, ctArc* b, void* cb_data)
{
  /*"a" will become the arc while "b" will be deleted.*/
  top::Mesh* mesh_ptr = (top::Mesh*) cb_data;
  FeatureSet* data_a = (FeatureSet*) a->data;
  FeatureSet* data_b = (FeatureSet*) b->data;
  data_a->v += data_b->v;
  data_a->hv += data_b->hv;
  size_t max_vertex_addr;
  size_t min_vertex_addr;
  if(mesh_ptr->data.Get(a->hi->i) > mesh_ptr->data.Get(b->hi->i))
    max_vertex_addr = a->hi->i;
  else
    max_vertex_addr = b->hi->i;

  if(mesh_ptr->data.Get(a->lo->i) > mesh_ptr->data.Get(b->lo->i))
    min_vertex_addr = b->lo->i;
  else
    min_vertex_addr = a->lo->i;

  data_a->p = std::abs((long) (mesh_ptr->data.Get(max_vertex_addr) - mesh_ptr->data.Get(min_vertex_addr)));
  free((FeatureSet*) b->data);
  b->data = NULL;
}

double arc_priority_proc(ctNode* leaf_node, void*)
{
  ctArc* leaf_arc = ctNode_leafArc(leaf_node);
  FeatureSet* data_arc = (FeatureSet*) leaf_arc->data;
  double v = data_arc->v;
  double hv = data_arc->hv;
  double p = data_arc->p;
  double arc_importance = 0.5 * sqrt(pow(hv * p, 2) + pow(v * p, 2) + pow(hv * v, 2));
  return arc_importance;
}

/* ctOpFluid - @Netto */

double half_std_avg_importance_normalized(ctBranch* b)
{
  if(b == NULL) return 0.0;

  FeatureSet* ptr = (FeatureSet*) b->data;
  if(ptr == NULL) return 0.0;

  return 0.5 * sqrt(pow(ptr->norm_hv * ptr->norm_p, 2) + pow(ptr->norm_v * ptr->norm_p, 2) + pow(ptr->norm_hv * ptr->norm_v, 2));
}

void calc_residue_flow(ctBranch* root_branch, double alpha_d, double rate_Q, top::Dataset* data)
{
  if(root_branch == NULL) return;

  if(root_branch->data == NULL)
    root_branch->data = (FeatureSet*) calloc(1, sizeof(FeatureSet));

  std::queue<ctBranch*> branch_queue;
  branch_queue.push(root_branch);

  do {
    ctBranch* curr_branch = branch_queue.front();
    branch_queue.pop();

    if(curr_branch->data == NULL) {
      curr_branch->data = (FeatureSet*) calloc(1, sizeof(FeatureSet));
    }

    FeatureSet* branch_data = (FeatureSet*) curr_branch->data;

    if(!branch_data->remove) {
      if (branch_data->num_children != 0) {
        branch_data->delta_h = (1.0*rate_Q*(((double)branch_data->p)/255.0))/(branch_data->porosity*((double)branch_data->num_children));
        if(branch_data->depth == 0) { //root node calcs
          branch_data->alpha_i = alpha_d*(1-branch_data->delta_h);
          branch_data->delta_alpha_i = alpha_d - branch_data->alpha_i;
        } else {
          ctBranch* parent_branch = curr_branch->parent;
          FeatureSet* parent_data = (FeatureSet*) parent_branch->data;
          branch_data->alpha_i = (alpha_d + parent_data->delta_alpha_i)*(1.0-branch_data->delta_h);
          branch_data->delta_alpha_i = (alpha_d + parent_data->delta_alpha_i)*branch_data->delta_h;
        }
      } else { // leaf node
        ctBranch* parent_branch = curr_branch->parent;
        FeatureSet* parent_data = (FeatureSet*) parent_branch->data;
        branch_data->alpha_i = (alpha_d + parent_data->delta_alpha_i)*(1.0 - 0);
        branch_data->delta_alpha_i = 0.0;
      }
      branch_data->alpha_i_j = branch_data->alpha_i*(half_std_avg_importance_normalized(curr_branch))*calc_gsd(curr_branch,data);
      //std::cout << "["<< branch_data->depth << "] Dh: " << branch_data->delta_h << " Ai: " << branch_data->alpha_i << " r: "
      //  << branch_data->delta_alpha_i << " Aij: " << branch_data->alpha_i_j << std::endl;
      //std::cout << branch_data->depth << " - " << branch_data->num_children << ", ";
      branch_data->alpha_lo = clamp<double>(calc_alpha_sum(curr_branch), 0.0, 1.0);

      branch_data->alpha_hi = clamp<double>(calc_alpha_sum(curr_branch) + branch_data->alpha_i_j, 0.0, 1.0);

      branch_data->alpha = (double*) calloc(256, sizeof(double));
      double* tf = calc_final_alpha(curr_branch, LINEAR);
      memcpy(branch_data->alpha, tf, 256 * sizeof(double));
      free(tf);
      tf = NULL;
      //std::cout << "     Alo: " << branch_data->alpha_lo << " Ahi: " << branch_data->alpha_hi << /* " Imp: " << std_avg_importance(curr_branch) << */ std::endl;
      //std::cout << "     Min scalar: " << branch_data->min_intensity << " Max scalar: " << branch_data->max_intensity << std::endl;
      //std::cout << " Opacity: ";
      //for(int i = 0; i < 256; i++)
      //  std::cout << branch_data->alpha[i] << " ";
      //std::cout << "\n\n";

    }

    for(ctBranch* c = curr_branch->children.head; c != NULL; c = c->nextChild) {
      FeatureSet* c_data = (FeatureSet*) c->data;
      if(!c_data->remove) {
        branch_queue.push(c);
      }
    }

  } while(!branch_queue.empty());
}

double* calc_final_alpha(ctBranch* b, TFShape shape)
{
  if(b == NULL) return nullptr;
  double* alpha_tf = (double*) calloc(256, sizeof(double));

  FeatureSet* b_data = (FeatureSet*) b->data;

  switch(shape) {
  case HAT:
    std::cout << "HAT shape chosen.\n";
    break;
  case TRIANGLE:
    std::cout << "TRIANGLE shape chosen.\n";
    break;
  case TRAPEZOID:
    std::cout << "TRAPEZOID shape chosen.\n";
    break;
  case HTRAPEZOID:
    std::cout << "HTRAPEZOID shape chosen.\n";
    break;
  case LINEAR:
  default:
    if(b_data->min_intensity == b_data->max_intensity)
      alpha_tf[b_data->min_intensity] = b_data->alpha_hi;
    else {
      double a = (b_data->alpha_hi - b_data->alpha_lo) / (b_data->max_intensity - b_data->min_intensity);
      double b = b_data->alpha_hi - a * b_data->max_intensity;
      for(size_t i = b_data->min_intensity; i <= b_data->max_intensity; i++)
        alpha_tf[i] = a * i + b;
    }
  }
  return alpha_tf;
}

double calc_gsd(ctBranch* b, top::Dataset* data) {
  if(b->parent != NULL) {
    FeatureSet* parent = (FeatureSet*) b->parent->data;
    if(parent->num_children > 1)
      return (((double) data->data->Get(b->saddle)) - parent->c_s_min) / (parent->c_s_max - parent->c_s_min);
  }
  return 1.0;
}

double calc_alpha_sum(ctBranch* b) {
  double sum = 0;
  ctBranch* tmp = b->parent;
  while (tmp != NULL) {
    FeatureSet* parent = (FeatureSet*) tmp->data;
    sum += parent->alpha_i_j;
    //free(parent);
    tmp = tmp->parent;
  }
  return sum;
}

void calc_saddle_min_max(ctBranch* root_branch, top::Dataset* data)
{
  if(root_branch == NULL) return;

  if(root_branch->data == NULL)
    root_branch->data = (FeatureSet*) calloc(1, sizeof(FeatureSet));

  std::queue<ctBranch*> branch_queue;
  branch_queue.push(root_branch);

  do {
    ctBranch* curr_branch = branch_queue.front();
    branch_queue.pop();

    if(curr_branch->data == NULL)
      curr_branch->data = (FeatureSet*) calloc(1, sizeof(FeatureSet));

    FeatureSet* branch_data = (FeatureSet*) curr_branch->data;
    for(ctBranch* c = curr_branch->children.head; c != NULL; c = c->nextChild) {
      FeatureSet* c_data = (FeatureSet*) c->data;
      if(!c_data->remove) {
        if(branch_data->num_children != 0) {
          if(data->data->Get(c->saddle) < branch_data->c_s_min) {
            branch_data->c_s_min = data->data->Get(c->saddle);
          }
          if (data->data->Get(c->saddle) > branch_data->c_s_max){
            branch_data->c_s_max = data->data->Get(c->saddle);
          }
        }
        branch_queue.push(c);
      }
    }
    if(!branch_data->remove && branch_data->num_children != 0) {
      std::cout << "nc: " << branch_data->num_children << " min: " << branch_data->c_s_min << " max: " << branch_data->c_s_max << std::endl;
    }
  } while(!branch_queue.empty());
}
