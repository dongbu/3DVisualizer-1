#ifndef TOPANALYZER_H
#define TOPANALYZER_H

#include "singleton.h"
#include "knlconfig.h"
#include "topdata.h"
#include "topmesh.h"
#include "ctfunc.h"

class TopAnalyzer : public Singleton<TopAnalyzer>
{
  friend class Singleton<TopAnalyzer>;

public:
  void init();
  bool buildContourTree();
  bool simplifyContourTree();
  bool flowOpacity(double flow_rate);
  bool createAlphaMap(std::string alpha_key);

  void analyzeCurrDataset(double flow_rate, std::string key);
  void analyzeDataset(knl::Dataset* data, double flow_rate, std::string key, std::string data_key);

  float flow_rate()
  {
    return m_flow_rate;
  }

  double avg_importance()
  {
    return m_avg_importance;
  }

  size_t tree_depth()
  {
    return m_tree_depth;
  }

private:
  ctContext* m_tourtre_ctx;
  ctBranch* m_root_branch;
  ctBranch** m_branch_map;
  top::Dataset* m_curr_dataset;
  float m_flow_rate;
  double m_avg_importance;
  size_t m_tree_depth;
};

#endif // TOPANALYZER_H
