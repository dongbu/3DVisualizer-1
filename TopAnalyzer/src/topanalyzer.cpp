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

void TopAnalyzer::init()
{
  Logger::getInstance()->warn("TopAnalyzer::Init() - Not implemented.");
}

bool TopAnalyzer::buildContourTree()
{
  using tbb::tick_count;
  using std::vector;
  using std::cout;
  using std::endl;

  reset();

  tick_count a;
  tick_count b;

  Logger::getInstance()->log("Building contour tree: " + DatasetManager::getInstance()->getCurrentKey());

  knl::Dataset* data = DatasetManager::getInstance()->getCurrent();
  assert(data != NULL);

  m_curr_dataset = new top::Dataset(*data);
  top::Mesh mesh(*m_curr_dataset);

  a = tick_count::now();
  vector<size_t> order;
  mesh.createGraph(order);
  b = tick_count::now();
  Logger::getInstance()->log("\tGraph created in " + std::to_string((b - a).seconds()) + " seconds");

  m_tourtre_ctx = ct_init(m_curr_dataset->size, &(order.front()), std_value, std_neighbors, &mesh);
  ct_vertexFunc(m_tourtre_ctx, &vertex_proc);
  ct_arcMergeFunc(m_tourtre_ctx, &arc_merge_proc);
  ct_priorityFunc(m_tourtre_ctx, &arc_priority_proc);
  ct_branchAllocator(m_tourtre_ctx, &BranchAlloc, &BranchFree);

  size_t valence = (size_t)pow(2, data->bytes_elem * 8);

  ct_maxValence(m_tourtre_ctx, valence );

  a = tbb::tick_count::now();
  ct_sweepAndMerge(m_tourtre_ctx);
  m_root_branch = ct_decompose(m_tourtre_ctx);
  m_branch_map = ct_branchMap(m_tourtre_ctx);
  b = tbb::tick_count::now();

  Logger::getInstance()->log("\tSweep and merge + decompose + branch map in " + std::to_string((b - a).seconds()) + " seconds");

  m_tree_depth = 0;
  calc_branch_depth(m_root_branch, &m_tree_depth, 0);

  Logger::getInstance()->log("\t" + std::to_string(count_branches(m_root_branch)) + " branches in the tree");

  a = tick_count::now();
  calc_branch_features(m_branch_map, m_curr_dataset);
  b = tick_count::now();
  Logger::getInstance()->log("\tFeatures calculated in " + std::to_string((b - a).seconds()) + " seconds");

  m_avg_importance = calc_avg_importance(m_root_branch, &std_avg_importance);
  calc_vertices_branch(m_branch_map, m_curr_dataset->size);

  treeBuilt(m_root_branch != NULL);
  return isContourTreeBuilt();
}

bool TopAnalyzer::simplifyContourTree()
{
  using tbb::tick_count;
  using std::vector;
  using std::cout;
  using std::endl;

  tick_count a;
  tick_count b;

  if(!isContourTreeBuilt())
    return false;

  a = tick_count::now();
  topSimplifyTree(m_tourtre_ctx, m_root_branch, *m_curr_dataset, &std_avg_importance, m_avg_importance / 10);
  b = tick_count::now();

  Logger::getInstance()->log("\tSimplification in " + std::to_string((b - a).seconds()) + " seconds");
  Logger::getInstance()->log("\t" + std::to_string(count_branches(m_root_branch)) + " branches in the tree");

  rebuild_branch_map(m_root_branch, m_branch_map);
  calc_branch_features(m_branch_map, m_curr_dataset);
  label_branches(m_root_branch);
  calc_branch_num_children(m_root_branch);

  m_tree_depth = 0;
  calc_branch_depth(m_root_branch, &m_tree_depth, 0);
  normalize_features(m_root_branch);

  treeSimplified(true);

  return isContourTreeSimplified();
}

bool TopAnalyzer::flowOpacity(double flow_rate)
{
  using tbb::tick_count;
  using std::vector;
  using std::cout;
  using std::endl;

  if(!isContourTreeBuilt())
    return false;

  m_flow_rate = static_cast<float>(flow_rate);

  tick_count a;
  tick_count b;

  a = tick_count::now();
  opacityFlowed(calc_residue_flow(m_root_branch, 1.f / static_cast<double>(m_tree_depth), m_flow_rate, m_curr_dataset));
  b = tick_count::now();
  Logger::getInstance()->log("\tResidue flow in " + std::to_string((b - a).seconds()) + " seconds");

  return isOpacityFlowed();
}

bool TopAnalyzer::createAlphaMap(std::string alpha_key)
{
  using tbb::tick_count;
  using std::vector;
  using std::cout;
  using std::endl;

  if(!isContourTreeBuilt() || !isOpacityFlowed())
    return false;

  tick_count a;
  tick_count b;

  a = tick_count::now();
  knl::Dataset* alpha_map = CreateAlphaDataset(*m_curr_dataset->data, m_branch_map);
  b = tick_count::now();
  Logger::getInstance()->log("\tAlpha map in " + std::to_string((b - a).seconds()) + " seconds");

  AlphaManager* ap = AlphaManager::getInstance();
  bool added = ap->add(alpha_key,
                       DatasetManager::getInstance()->getCurrentKey(),
                       alpha_map);

  bool active = ap->setActive(alpha_key);

  return added && active;
}

void TopAnalyzer::testRun(double flow_rate, float avg_mult)
{
  using tbb::tick_count;
  using std::vector;
  using std::cout;
  using std::endl;

  tick_count a;
  tick_count b;

  //Logger::getInstance()->log("Building contour tree: " + DatasetManager::getInstance()->getCurrentKey());
  cout << DatasetManager::getInstance()->getCurrentKey() + ",";

  knl::Dataset* data = DatasetManager::getInstance()->getCurrent();
  assert(data != NULL);

  top::Dataset* topd = new top::Dataset(*data);
  top::Mesh* mesh = new top::Mesh(*topd);

  a = tick_count::now();
  vector<size_t> order;
  mesh->createGraph(order);
  b = tick_count::now();
//  Logger::getInstance()->log("\tGraph created in " + std::to_string((b - a).seconds()) + " seconds");
  cout << std::to_string((b - a).seconds()) + ",";

  ctContext* ctx = ct_init(topd->size, &(order.front()), std_value, std_neighbors, mesh);
  ct_vertexFunc(ctx, &vertex_proc);
  ct_arcMergeFunc(ctx, &arc_merge_proc);
  ct_priorityFunc(ctx, &arc_priority_proc);
  ct_branchAllocator(ctx, &BranchAlloc, &BranchFree);

  a = tbb::tick_count::now();
  ct_sweepAndMerge(ctx);
  ctBranch* root_branch = ct_decompose(ctx);
  ctBranch** branch_map = ct_branchMap(ctx);
  b = tbb::tick_count::now();

//  Logger::getInstance()->log("\tSweep and merge + decompose + branch map in " + std::to_string((b - a).seconds()) + " seconds");
  cout << std::to_string((b - a).seconds()) + ",";

  size_t tree_depth = 0;
  calc_branch_depth(root_branch, &tree_depth, 0);

//  Logger::getInstance()->log("\t" + std::to_string(count_branches(root_branch)) + " branches in the tree");
  cout << std::to_string(count_branches(root_branch)) + ",";

  a = tick_count::now();
  calc_branch_features(branch_map, topd);
  b = tick_count::now();
//  Logger::getInstance()->log("\tFeatures calculated in " + std::to_string((b - a).seconds()) + " seconds");
  cout << std::to_string((b - a).seconds()) + ",";

  double avg_imp = calc_avg_importance(root_branch, &std_avg_importance);
  calc_vertices_branch(branch_map, topd->size);

//  Logger::getInstance()->log("\tAverage importance = " + std::to_string(avg_imp));
  cout << std::to_string(avg_imp * avg_mult) + ",";

  //////////////////////////////////////////////////
  a = tick_count::now();
  topSimplifyTree(ctx, root_branch, *topd, &std_avg_importance, avg_imp * avg_mult);
  b = tick_count::now();

//  Logger::getInstance()->log("\tSimplification in " + std::to_string((b - a).seconds()) + " seconds");
//  Logger::getInstance()->log("\t" + std::to_string(count_branches(root_branch)) + " branches in the tree");

  cout << std::to_string((b - a).seconds()) + ",";
  cout << std::to_string(count_branches(root_branch)) + ",";

  rebuild_branch_map(root_branch, branch_map);
  calc_branch_features(branch_map, topd);
  label_branches(root_branch);
  calc_branch_num_children(root_branch);

  tree_depth = 0;
  calc_branch_depth(root_branch, &tree_depth, 0);
  normalize_features(root_branch);
  //////////////////////////////////////////////////

  m_flow_rate = static_cast<float>(flow_rate);

  a = tick_count::now();
  opacityFlowed(calc_residue_flow(root_branch, 1.f / static_cast<double>(tree_depth), m_flow_rate, topd));
  b = tick_count::now();
//  Logger::getInstance()->log("\tResidue flow in " + std::to_string((b - a).seconds()) + " seconds");
  cout << std::to_string((b - a).seconds()) + ",";


  //////////////////////////////////////////////////

  a = tick_count::now();
  knl::Dataset* alpha_map = CreateAlphaDataset(*topd->data, branch_map);
  b = tick_count::now();
//  Logger::getInstance()->log("\tAlpha map in " + std::to_string((b - a).seconds()) + " seconds");
  cout << std::to_string((b - a).seconds()) << endl;

  //////////////////////////////////////////////////
  //////////////////////////////////////////////////

  delete alpha_map;
  delete topd;
  delete mesh;
  ct_cleanup(ctx);
  free(root_branch);
  free(branch_map);
  ctx = NULL;
}
