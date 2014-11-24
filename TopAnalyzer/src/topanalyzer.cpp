#include "topanalyzer.h"
#include "logger.h"
#include "topdata.h"
#include "topmesh.h"
#include "ctfunc.h"
#include "simplification.h"
#include "alphamanager.h"

#include <queue>
#include <tbb/tbb.h>

extern "C"
{
#include <tourtre.h>
}

static knl::Dataset* CreateAlphaDataset(knl::Dataset& dataset, ctBranch** branch_map)
{
  if(branch_map == NULL) {
    Logger::getInstance()->error("knl::Dataset* CreateAlphaDataset - branch_map is NULL. Returning now.");
    return NULL;
  }

  size_t num_elements = dataset.width * dataset.height * dataset.slices;

  knl::Dataset* alpha_map = new knl::Dataset;
  alpha_map->width = dataset.width;
  alpha_map->height = dataset.height;
  alpha_map->slices= dataset.slices;
  alpha_map->bytes_elem = sizeof(GLfloat);
  alpha_map->data = (GLfloat*) calloc(num_elements, sizeof(GLfloat));

  GLfloat* alpha_ptr = (GLfloat*) alpha_map->data;
  for(size_t i = 0; i < num_elements; i++) {
    FeatureSet* branch_data = (FeatureSet*) branch_map[i]->data;

    size_t data_val = dataset.get(i);

    *alpha_ptr = (GLfloat) branch_data->alpha[data_val];
    alpha_ptr++;
  }

  alpha_map->loaded(true);
  return alpha_map;
}

DEPRECATED static knl::Dataset* CreateVTBMap(ctBranch** vtb_map, size_t dim[], size_t last_label)
{
  if(vtb_map == NULL) {
    Logger::getInstance()->error("knl::Dataset* CreateVTBMap -> invalid vertex-branch map.");
    return NULL;
  }

  size_t num_elem = dim[0] * dim[1] * dim[2];

  knl::Dataset* vtb = new knl::Dataset;
  vtb->width = dim[0];
  vtb->height = dim[1];
  vtb->slices = dim[2];
  vtb->bytes_elem = sizeof(GLfloat);
  vtb->data = calloc(num_elem, sizeof(GLfloat));

  GLfloat* ptr = (GLfloat*)vtb->data;
  for(size_t i = 0; i < num_elem; i++, ptr++) {
    FeatureSet* branch_data = (FeatureSet*) vtb_map[i]->data;
    *ptr = static_cast<GLfloat>(branch_data->label / static_cast<GLfloat>(last_label));
  }

  ptr = nullptr;

  return vtb;
}

DEPRECATED static TFunction* CreateOpTF(ctBranch* root_branch, size_t rows)
{
  if(root_branch == NULL || rows == 0) {
    Logger::getInstance()->error("TFunction* CreateOpTF -> invalid parameter(s).");
    return NULL;
  }

  TFunction* tf = new TFunction;
  tf->rows = rows;
  tf->num_channels = 1;
  tf->bytes_elem = sizeof(GLubyte);
  tf->data = calloc(tf->width() * rows * tf->num_channels, sizeof(GLubyte));

  std::queue<ctBranch*> b_queue;
  b_queue.push(root_branch);

  do {
    ctBranch* curr_branch = b_queue.front();
    b_queue.pop();

    FeatureSet* branch_data = (FeatureSet*) curr_branch->data;

    GLubyte tmp_tf[256];
    memset(tmp_tf, 0, 256 * sizeof(GLubyte));
    for(int i = 0; i < 256; i++)
      tmp_tf[i] = static_cast<GLubyte>(branch_data->alpha[i] * 255);

    GLubyte* tf_begin = (GLubyte*) tf->data + branch_data->label * 256;
    memcpy(tf_begin, tmp_tf, 256 * sizeof(GLubyte));
    memset(tmp_tf, 0, 256 * sizeof(GLubyte));

    for(ctBranch* c = curr_branch->children.head; c != NULL; c = c->nextChild) {
      FeatureSet* c_data = (FeatureSet*) c->data;
      if(!c_data->remove)
        b_queue.push(c);
    }

  } while(!b_queue.empty());

  return tf;
}

static ctBranch* BranchAlloc(void*)
{
  ctBranch* b = (ctBranch*) calloc(1, sizeof(ctBranch));
  b->data = (FeatureSet*) calloc(1, sizeof(FeatureSet));
  ((FeatureSet*) b->data)->min_intensity = 4096;
  return b;
}

static void BranchFree(ctBranch* b, void*)
{
  if(b->data != NULL) {
    memset(b->data, 0, sizeof(FeatureSet));
    free(b->data);
    b->data = NULL;
  }

  ctBranch* parent = b->parent;
  if(parent != NULL) {
    ctBranchList_remove(&parent->children, b);
    FeatureSet* parent_data = (FeatureSet*) parent->data;
    parent_data->num_children--;
  }

  memset(b, 0, sizeof(ctBranch));
  free(b);
  b = NULL;
}

void TopAnalyzer::Init()
{
  Logger::getInstance()->warn("TopAnalyzer::Init() - Not implemented.");
}

void TopAnalyzer::AnalyzeCurrDataset(double flow_rate, std::string key)
{
  knl::Dataset* data = DatasetManager::getInstance()->getCurrent();
  std::string data_key = DatasetManager::getInstance()->getCurrentKey();
  AnalyzeDataset(data, flow_rate, key, data_key);
}

void TopAnalyzer::AnalyzeDataset(knl::Dataset* data, double flow_rate, std::string key, std::string data_key)
{
  assert(data != NULL);
  tbb::tick_count a;
  tbb::tick_count b;

  top::Dataset topd(*data);
  top::Mesh mesh(topd);
  std::cout << "    Analyzing " << key << std::endl;

  a = tbb::tick_count::now();
  std::vector<size_t> order;
  mesh.createGraph(order);
  b = tbb::tick_count::now();
  std::cout << "\tGraph created in " << (b - a).seconds() << " seconds" << std::endl;

  ctContext* ctx = ct_init(topd.size, &(order.front()), std_value, std_neighbors, &mesh);
  ct_vertexFunc(ctx, &vertex_proc);
  ct_arcMergeFunc(ctx, &arc_merge_proc);
  ct_priorityFunc(ctx, &arc_priority_proc);
  ct_branchAllocator(ctx, &BranchAlloc, &BranchFree);

  a = tbb::tick_count::now();
  ct_sweepAndMerge(ctx);
  ctBranch* root_branch = ct_decompose(ctx);
  ctBranch** branch_map = ct_branchMap(ctx);
  b = tbb::tick_count::now();
  std::cout << "\tSweep and merge + decompose + branch map in " << (b - a).seconds() << " seconds" << std::endl;

  //zero_branches(root_branch);
  size_t max_depth = 0;
  calc_branch_depth(root_branch, &max_depth, 0);

  std::cout << "\t" << count_branches(root_branch) << " branches before simplification." << std::endl;
  //std::cout << "Tree depth = " << max_depth << std::endl;

  a = tbb::tick_count::now();
  calc_branch_features(branch_map, &topd);
  b = tbb::tick_count::now();
  std::cout << "\tFeatures calculated in " << (b - a).seconds() << " seconds" << std::endl;

  double avg_importance = calc_avg_importance(root_branch, &std_avg_importance);
  calc_vertices_branch(branch_map, topd.size);

  a = tbb::tick_count::now();
  topSimplifyTree(ctx, root_branch, branch_map, topd, &std_avg_importance, avg_importance / 10);
  //  topSimplifyTreeZhou(ctx, root_branch, branch_map, topd, &std_avg_importance, avg_importance / 1000);
  b = tbb::tick_count::now();
  std::cout << "\tSimplification in " << (b - a).seconds() << " seconds" << std::endl;
  std::cout << "\t" << count_branches(root_branch) << " branches after the simplification." << std::endl;

  rebuild_branch_map(root_branch, branch_map);
  calc_branch_features(branch_map, &topd);
  label_branches(root_branch);
  calc_branch_num_children(root_branch);

  max_depth = 0;
  calc_branch_depth(root_branch, &max_depth, 0);
  normalize_features(root_branch);

  a = tbb::tick_count::now();
  calc_residue_flow(root_branch, 1.f / static_cast<double>(max_depth), flow_rate, &topd);
  b = tbb::tick_count::now();
  std::cout << "\tResidue flow in " << (b - a).seconds() << " seconds" << std::endl;

  a = tbb::tick_count::now();
  knl::Dataset* alpha_map = CreateAlphaDataset(*data, branch_map);
  b = tbb::tick_count::now();
  std::cout << "\tAlpha map in " << (b - a).seconds() << " seconds" << std::endl;

  AlphaManager::getInstance()->add(key, data_key, alpha_map);

  delete alpha_map;

  ct_cleanup(ctx);
  free(root_branch);
  free(branch_map);
  ctx = NULL;

}
