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
  bool flowOpacity();
  bool createAlphaMap();

  void analyzeCurrDataset(double flow_rate, std::string key);
  void analyzeDataset(knl::Dataset* data, double flow_rate, std::string key, std::string data_key);

private:
  ctContext* m_ctx;
  ctBranch* m_rootBranch;
  ctBranch** m_branchMap;
  top::Dataset* m_currDataset;
  double m_avgImp;
  size_t m_treeDepth;
};

#endif // TOPANALYZER_H
