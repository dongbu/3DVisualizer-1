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
  bool Add(std::string key, Dataset* data);
  void SetActive(std::string key);
  Dataset* Get(std::string key);
  Dataset* GetCurrent();
  void FreeResources();

private:
  std::map<std::string, Dataset*> m_datasetMap;
  std::string m_activeKey;
  std::string m_dataPath;

};

#endif // DATASETMANAGER_H
