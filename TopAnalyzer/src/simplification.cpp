#include "simplification.h"
#include "ctfunc.h"

#include <stack>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <tbb/parallel_for.h>

/**
 * @brief merge_branches Merges to contour tree branches and accumulates their
 * features.
 * @param a The first branch. This branch is the one that will remain on the
 * tree.
 * @param b The second branch. This is the branch that is supposed to be deleted
 *  in the simplification process.
 * @param data A pointer to the dataset.
 */
static void merge_branches(ctBranch* a, ctBranch* b, top::Dataset* data)
{
  if(!a || ! b || !data)
    return;

  FeatureSet* a_d = (FeatureSet*) a->data;
  FeatureSet* b_d = (FeatureSet*) b->data;

  a_d->v += b_d->v;
  a_d->hv += b_d->hv;
  a_d->p = std::abs((long) (data->data->get(a->extremum) - data->data->get(a->saddle)));

  size_t sz = a_d->vertices.size();
  a_d->vertices.resize(sz + b_d->vertices.size());
  for(size_t i = 0; i < b_d->vertices.size(); ++i, ++sz) {
    a_d->vertices[sz] = b_d->vertices[i];
  }
}

/**
 * @brief The point_to_parent class
 * This class is used as a functor to the tbb::parallel_for function. When a
 * branch is marked for removal by the simplification process, it's vertices
 * must be pointed to another branch. In this case, the parent branch.
 */
class point_to_parent
{
  ctBranch** branch_map;
  ctBranch* to_remove;

public:
  point_to_parent(ctBranch** a, ctBranch* to_rem) : branch_map(a), to_remove(to_rem)
  {}

  void operator()(const tbb::blocked_range<size_t>& range) const {

    FeatureSet* rem_data = (FeatureSet*) to_remove->data;

    for(size_t i = range.begin(); i != range.end(); i++) {
      FeatureSet* bmap_data = (FeatureSet*) branch_map[i]->data;
      if(bmap_data->label == rem_data->label) {
        branch_map[i] = to_remove->parent;
      }
    }
  }
};

/**
 * @brief queue_leaves This function finds all of the tree's leaves and insert
 * them in a queue.
 * @param root_branch The tree's root branch.
 * @return A vector containing the tree's leaves.
 */
static std::vector<ctBranch*> queue_leaves(ctBranch* root_branch)
{
  std::vector<ctBranch*> leaves;
  if(root_branch == NULL)
    return leaves;

  std::stack<ctBranch*> stack;
  stack.push(root_branch);

  do {
    ctBranch* curr_branch = stack.top();
    FeatureSet* branch_data = (FeatureSet*) curr_branch->data;

    stack.pop();

    if(curr_branch->children.head == NULL && branch_data->visited == false)
      leaves.push_back(curr_branch);

    for(ctBranch* c = curr_branch->children.head; c != NULL; c = c->nextChild) {
      stack.push(c);
    }

  } while(!stack.empty());

  return leaves;
}

static void point_parent(ctBranch* to_remove, ctBranch** branch_map, size_t map_size)
{
  FeatureSet* rem_data = (FeatureSet*) to_remove->data;
  FeatureSet* par_data = (FeatureSet*) to_remove->parent->data;

  std::vector<size_t> new_vertices(rem_data->vertices.size() + par_data->vertices.size());
  std::merge(rem_data->vertices.begin(), rem_data->vertices.end(),
   	     par_data->vertices.begin(), par_data->vertices.end(),
   	     new_vertices.begin());

   par_data->vertices = new_vertices;
   tbb::parallel_for(tbb::blocked_range<size_t>(0, map_size), point_to_parent(branch_map, to_remove));
}

/**
 * @brief The leaf_comp class
 * This class is a functor to help sorting the contour tree's leaves by their
 * importance.
 */
class leaf_comp
{
  double(*imp_cb)(ctBranch*);
public:
  leaf_comp(double(*cb)(ctBranch*)) : imp_cb(cb)
  {}
  bool operator()(const ctBranch* a, const ctBranch* b) const
  {
    return imp_cb(const_cast<ctBranch*>(a)) > imp_cb(const_cast<ctBranch*>(b));
  }
};

void topSimplifyTree(ctContext* ctx,
         ctBranch* root_branch,
		     top::Dataset& topd,
		     double(*importance_cb)(ctBranch*),
		     double thresh)
{
  using namespace std;

  if(count_branches(root_branch) == 1)
    return;

  size_t idx;
  vector<ctBranch*> leaves;

  do {

    leaves.clear();
    leaves = queue_leaves(root_branch);
    std::sort(leaves.begin(), leaves.end(), leaf_comp(importance_cb));

    for(idx = 0; idx < leaves.size(); ++idx) {
      if(leaves[idx] == NULL) continue;
      if(importance_cb(leaves[idx]) < thresh) break;

      ((FeatureSet*) leaves[idx]->data)->visited = true;
    }

    size_t i = idx;
    while(i < leaves.size()) {
      merge_branches(leaves[i]->parent, leaves[i], &topd);
      ctBranch_delete(leaves[i], ctx);
      ++i;
    }

  } while(idx != leaves.size());
}

class ReduceToSaddle
{
  ctBranch** branch_map;
  ctBranch* to_remove;
  top::Dataset& top_data;

public:

  ReduceToSaddle(ctBranch** a, ctBranch* to_rem, top::Dataset& t) : branch_map(a), to_remove(to_rem), top_data(t)
  {}

  void operator()(const tbb::blocked_range<size_t>& range) const {

    FeatureSet* rem_data = (FeatureSet*) to_remove->data;

    for(size_t i = range.begin(); i != range.end(); i++) {
      FeatureSet* bmap_data = (FeatureSet*) branch_map[i]->data;
      if(bmap_data->label == rem_data->label) {
        size_t saddle = top_data.data->get(to_remove->saddle);
        top_data.data->set(i, saddle);
      }
    }
  }
};

static void reduce_to_saddle(ctBranch* to_remove, ctBranch** branch_map, top::Dataset& top_data)
{
  //FeatureSet* rem_data = (FeatureSet*) to_remove->data;

  tbb::parallel_for(tbb::blocked_range<size_t>(0, top_data.size), ReduceToSaddle(branch_map, to_remove, top_data));
}

void topSimplifyTreeZhou(ctContext* ctx, ctBranch* root_branch, ctBranch** branch_map, top::Dataset& top_data, double(*importance_cb)(ctBranch*), double thresh)
{
  using namespace std;

  if(count_branches(root_branch) == 1)
    return;

  size_t idx;
  vector<ctBranch*> leaves;

  do {
    leaves.clear();
    leaves = queue_leaves(root_branch);
    std::sort(leaves.begin(), leaves.end(), leaf_comp(importance_cb));

    for(idx = 0; idx < leaves.size(); ++idx) {
      if(leaves[idx] == NULL)  continue;
      if(importance_cb(leaves[idx]) < thresh) break;
      ((FeatureSet*) leaves[idx]->data)->visited = true;
    }

    size_t i = idx;
    while(i < leaves.size()){
      reduce_to_saddle(leaves[i], branch_map, top_data);
      point_parent(leaves[i], branch_map, top_data.size);
      ctBranch_delete(leaves[i], ctx);
      ++i;
    }
  } while(idx != leaves.size());
}
