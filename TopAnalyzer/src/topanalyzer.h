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

  void AnalyzeCurrDataset();
  void AnalyzeDataset(Dataset* data);

};

#endif // TOPANALYZER_H
