#ifndef RENDERERMANAGER_H
#define RENDERERMANAGER_H

#include "singleton.h"
#include <cstdlib>
#include <map>
#include <string>

class Renderer;

class RendererManager : public Singleton<RendererManager>
{
  friend class Singleton<RendererManager>;

public:
  bool Init();
  bool Add(std::string key, Renderer* r);
  void SetActive(std::string key);
  Renderer* Get(std::string key);
  Renderer* GetCurrent();
  void FreeResources();

  std::string GetCurrentKey()
  {
    return m_activeKey;
  }

private:
  std::map<std::string, Renderer*> m_rendererMap;
  std::string m_activeKey;
};

#endif // RENDERERMANAGER_H
