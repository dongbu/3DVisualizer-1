#ifndef ALPHA_MANAGER_H
#define ALPHA_MANAGER_H

#include <map>
#include <string>
#include "singleton.h"
#include "dataset.h"

class AlphaManager : public Singleton<AlphaManager>
{
  friend class Singleton<AlphaManager>;

public:
  bool Init(std::string path);
  bool Add(std::string key, knl::Dataset* vtb);
  void SetActive(std::string key, GLenum tex_unit = GL_TEXTURE0);
  knl::Dataset* Get(std::string key);
  knl::Dataset* GetCurrent();
  void FreeResources();

private:
  std::map<std::string, knl::Dataset*> m_vtbMap;
  std::string m_activeKey;
  std::string m_vtbPath;
  
};

#endif // ALPHA_MANAGER_H
