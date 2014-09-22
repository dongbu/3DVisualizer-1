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
  bool Add(std::string key, knl::Dataset* data);
  void SetActive(std::string key, GLenum tex_unit = GL_TEXTURE0);
  knl::Dataset* Get(std::string key);
  knl::Dataset* GetCurrent();
  void FreeResources();

private:
  std::map<std::string, knl::Dataset*> m_datasetMap;
  std::string m_activeKey;
  std::string m_dataPath;

};

#endif // DATASETMANAGER_H
