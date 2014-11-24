#ifndef TFMANAGER_H
#define TFMANAGER_H

#include <map>
#include <string>
#include <vector>

#include "singleton.h"
#include "tfunction.h"

class TFManager : public Singleton<TFManager>
{
  friend class Singleton<TFManager>;
public:
  bool init(std::string path);
  bool add(std::string key, TFunction* tf);
  bool setActive(std::string key, GLenum tex_unit = GL_TEXTURE0);
  TFunction* get(std::string key);
  TFunction* getCurrent();
  void freeResources();

  std::vector<std::string> getKeys()
  {
    std::vector<std::string> v;
    for(auto it = m_funcMap.begin(); it != m_funcMap.end(); ++it)
      v.push_back(it->first);
    return v;
  }

private:
  std::map<std::string, TFunction*> m_funcMap;
  std::string m_activeKey;
  std::string m_tfPath;
};

#endif // TFMANAGER_H
