#ifndef TOPANALYZER_H
#define TOPANALYZER_H

#include "singleton.h"
#include "knlconfig.h"

class TopAnalyzer : public Singleton<TopAnalyzer>
{
  friend class Singleton<TopAnalyzer>;
public:
  void Init();
  bool BuildContourTree();

  void AnalyzeCurrDataset(double flow_rate, std::string key);
  void AnalyzeDataset(knl::Dataset* data, double flow_rate, std::string key);
};

#endif // TOPANALYZER_H
