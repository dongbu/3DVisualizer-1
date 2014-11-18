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
  bool init(std::string path);
  bool add(std::string key, knl::Dataset* data);
  void setActive(std::string key, GLenum tex_unit = GL_TEXTURE0);
  knl::Dataset* get(std::string key);
  knl::Dataset* getCurrent();
  std::string getCurrentKey()
  {
    return m_activeKey;
  }
  void freeResources();
  

private:
  std::map<std::string, knl::Dataset*> m_datasetMap;
  std::string m_activeKey;
  std::string m_dataPath;
};

#endif // DATASETMANAGER_H
