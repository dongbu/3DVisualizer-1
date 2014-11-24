#ifndef DATASETMANAGER_H
#define DATASETMANAGER_H

#include <map>
#include <vector>
#include <string>

#include "singleton.h"
#include "dataset.h"

class DatasetManager : public Singleton<DatasetManager>
{
  friend class Singleton<DatasetManager>;
  
public:
  bool init(std::string path);
  bool add(std::string key, knl::Dataset* data);
  bool setActive(std::string key, GLenum tex_unit = GL_TEXTURE0);
  knl::Dataset* get(std::string key);
  knl::Dataset* getCurrent();
  void freeResources();

  std::string getCurrentKey()
  {
    return m_activeKey;
  }

  std::vector<std::string> getKeys()
  {
    std::vector<std::string> v;
    for(auto it = m_datasetMap.begin(); it != m_datasetMap.end(); ++it)
      v.push_back(it->first);
    return v;
  }

  std::string getPath()
  {
    return m_dataPath;
  }

private:
  std::map<std::string, knl::Dataset*> m_datasetMap;
  std::string m_activeKey;
  std::string m_dataPath;
};

#endif // DATASETMANAGER_H
