#ifndef ALPHA_MANAGER_H
#define ALPHA_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include "singleton.h"
#include "dataset.h"

class AlphaManager : public Singleton<AlphaManager>
{
  friend class Singleton<AlphaManager>;

public:
  bool init(std::string path);
  bool add(std::string key, std::string data_key, knl::Dataset* alpha_map);
  bool setActive(std::string key, GLenum tex_unit = GL_TEXTURE0);
  bool saveCurrent();
  knl::Dataset* get(std::string key);
  knl::Dataset* getCurrent();
  void freeResources();

  std::string getPath()
  {
    return m_alphaPath;
  }

  std::string getCurrentKey()
  {
    return m_activeKey;
  }

  std::vector<std::string> getKeys()
  {
    std::vector<std::string> v;
    for(auto it = m_alphaMap.begin(); it != m_alphaMap.end(); ++it)
      v.push_back(it->first);
    return v;
  }

private:
  std::map<std::string, knl::Dataset*> m_alphaMap;
  std::map<std::string, std::string> m_alphaDataMap;

  std::string m_activeKey;
  std::string m_alphaPath;
};

#endif // ALPHA_MANAGER_H
