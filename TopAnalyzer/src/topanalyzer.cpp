#include "topanalyzer.h"
#include "logger.h"
#include "topdata.h"
#include "topmesh.h"
#include "ctfunc.h"

extern "C"
{
#include <tourtre.h>
};

static ctBranch* BranchAlloc(void*)
{
  return (ctBranch*) calloc(1, sizeof(ctBranch));
}

static void BranchFree(ctBranch* b, void*)
{
  if(b->data != NULL) {
    free(b->data);
    b->data = NULL;
  }
  free(b);
  b = NULL;
}

void TopAnalyzer::Init()
{
  Logger::getInstance()->warn("TopAnalyzer::Init() - Not implemented.");
}

void TopAnalyzer::AnalyzeCurrDataset()
{
  knl::Dataset* data = DatasetManager::getInstance()->GetCurrent();
  AnalyzeDataset(data);
}

void TopAnalyzer::AnalyzeDataset(knl::Dataset* data)
{
  assert(data != NULL);
  Logger::getInstance()->warn("TopAnalyzer::AnalyzeDataset() - Not implemented.");
  top::Dataset topd(*data);
  top::Mesh mesh(topd);

  std::vector<size_t> order;
  mesh.createGraph(order);

  ctContext* ctx = ct_init(topd.size, &(order.front()), std_value, std_neighbors, &mesh);
  ct_vertexFunc(ctx, &vertex_proc);
  ct_arcMergeFunc(ctx, &arc_merge_proc);
  ct_priorityFunc(ctx, &arc_priority_proc);

  ct_branchAllocator(ctx, &BranchAlloc, &BranchFree);

  ct_sweepAndMerge(ctx);
  ctBranch* root_branch = ct_decompose(ctx);
  ctBranch** branch_map = ct_branchMap(ctx);



  /*
  zero_branches(root_branch);
  size_t max_depth = 0;
  calc_branch_depth(root_branch, &max_depth, 0);

  cout << count_branches(root_branch) << " branches before simplification." << endl;
  cout << "Tree depth = " << max_depth << endl;

  //    tbb::tick_count a = tbb::tick_count::now();
  calc_branch_features(branch_map, &data);
  //    tbb::tick_count b = tbb::tick_count::now();
  //    cout << (b - a).seconds() << endl;

  double avg_importance = calc_avg_importance(root_branch, &std_avg_importance);
  simplify_tree_dfs(root_branch, branch_map, data.totalSize, &std_avg_importance, avg_importance / 10000);
  //    outputTree(std::cout, root_branch);
  calc_branch_features(branch_map, &data);
  int last_label = label_branches(root_branch);
  //    outputTree(std::cout, root_branch);

  calc_branch_num_children(root_branch);

  max_depth = 0;
  calc_branch_depth(root_branch, &max_depth, 0);
  cout << count_branches(root_branch) << " branches after simplification." << endl;
  cout << "Tree depth = " << max_depth << "\nOpacity per depth level = " << opacity_max / max_depth << endl;
  normalize_features(root_branch);

  //    std::ofstream out_file;
  //    out_file.open(path + "-vtb");
  //    outputTree(out_file, root_branch);
  //    out_file.close();

  //calc_gsd(root_branch, &data);
  calc_residue_flow(root_branch, opacity_max / (double) max_depth, 300.0, &data);

  save_transfer_functions(root_branch,
  "/home/guilherme/Pictures/datasets/transfer-functions/nucleon." + std::to_string(count_branches(root_branch)) + ".uint8",
  last_label);

  save_vertex_branch_volume(branch_map,
  "/home/guilherme/Pictures/datasets/vertex-branch-maps/nucleon.41x41x41.uint8",
  data.size[0], data.size[1], data.size[2]);

  ct_cleanup(ctx);
  free(root_branch);
  free(branch_map);
  ctx = NULL;0*/












  //TFManager::getInstance()->Add("tf1", generated_tf);
  //TFManager::getInstance()->SetActive(generated_tf, GL_TEXTURE1);

}