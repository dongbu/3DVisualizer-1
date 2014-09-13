#ifndef DATASETMANAGER_H
#define DATASETMANAGER_H

#include <map>
#include <string>
#include "singleton.h"
#include "dataset.h"

class DatasetManager : public Singleton<DatasetManager>
{
  friend class Singleton<DatasetManager>;

public:
  void AddDataset(std::string key, Dataset* data);
  void FreeResources();

private:
  std::map<std::string, Dataset*> m_datasetMap;

};

#endif // DATASETMANAGER_H
