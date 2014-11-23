#ifndef RENDERERMANAGER_H
#define RENDERERMANAGER_H

#include <cstdlib>
#include <map>
#include <string>

#include "singleton.h"
#include "renderer.h"

class RendererManager : public Singleton<RendererManager>
{
  friend class Singleton<RendererManager>;

public:
  bool init();
  bool add(std::string key, Renderer* r);
  void setActive(std::string key);
  Renderer* get(std::string key);
  Renderer* getCurrent();
  void freeResources();

  std::string getCurrentKey()
  {
    return m_activeKey;
  }

private:
  std::map<std::string, Renderer*> m_rendererMap;
  std::string m_activeKey;
};

#endif // RENDERERMANAGER_H
