#ifndef TOPANALYZER_H
#define TOPANALYZER_H

#include "singleton.h"
#include "knlconfig.h"
#include "topdata.h"
#include "topmesh.h"
#include "ctfunc.h"

/**
 * @brief The TopAnalyzer class
 * This class manages the data structures needed to perform the topological
 * analysis of the dataset.
 *
 * This class instance holds the libtourtre's context, the root branch and
 * branch map produced during the analysis. There is also a pointer to the
 * dataset being analyzed.
 *
 * The parameters needed for the analysis, namely the flow rate and
 * simplification threshold are set during the corresponding steps (opacity flow
 * and simplification).
 */
class TopAnalyzer : public Singleton<TopAnalyzer>
{
  friend class Singleton<TopAnalyzer>;

public:
  void init();
  bool buildContourTree();
  bool simplifyContourTree(double threshold, bool reduce_saddle = false);
  bool flowOpacity(double flow_rate);
  bool createAlphaMap(std::string alpha_key);

  void testRun(double flow_rate, float avg_mult);

  bool isContourTreeBuilt()
  {
    return m_tree_built;
  }

  bool isContourTreeSimplified()
  {
    return m_tree_simplified;
  }

  bool isOpacityFlowed()
  {
    return m_op_flowed;
  }

  float getFlowRate()
  {
    return m_flow_rate;
  }

  double getAvgImportance()
  {
    return m_avg_importance;
  }

  size_t getTreeDepth()
  {
    return m_tree_depth;
  }

protected:
  void treeBuilt(bool b)
  {
    m_tree_built = b;
  }

  void treeSimplified(bool b)
  {
    m_tree_simplified = b;
  }

  void opacityFlowed(bool b)
  {
    m_op_flowed = b;
  }

  void reset()
  {
    treeBuilt(false);
    treeSimplified(false);
    opacityFlowed(false);
    m_flow_rate = 300.f;
    m_avg_importance = 0.0;
    m_threshold = 0.0;
    m_tree_depth = 0;
  }

private:
  ctContext* m_tourtre_ctx;
  ctBranch* m_root_branch;
  ctBranch** m_branch_map;
  top::Dataset* m_curr_dataset;

  float m_flow_rate;
  double m_avg_importance;
  double m_threshold;
  size_t m_tree_depth;

  bool m_tree_built;
  bool m_tree_simplified;
  bool m_op_flowed;
};

#endif // TOPANALYZER_H
