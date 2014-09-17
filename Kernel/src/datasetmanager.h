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
  bool Init(std::string path);
  void AddDataset(std::string key, Dataset* data);
  void SetActiveDataset(std::string key);
  Dataset* GetDataset(std::string key);
  Dataset* GetCurrentDataset();
  void FreeResources();

private:
  std::map<std::string, Dataset*> m_datasetMap;
  std::string m_activeKey;
  std::string m_dataPath;

};

#endif // DATASETMANAGER_H
