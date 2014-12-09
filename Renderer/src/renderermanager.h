#ifndef RENDERERMANAGER_H
#define RENDERERMANAGER_H

#include <cstdlib>
#include <map>
#include <string>

#include "singleton.h"
#include "renderer.h"

/**
 * @brief The RendererManager class
 * This class' instance is responsible for managing the renderers.
 *
 * The init method doesn't do anything. It's just declared to follow the same
 * standard implemented in the other managers. To use a Renderer, one must be
 * created and set as active by the setActive function.
 *
 * The renderers are responsible for their own resources, meaning that they will
 * create their resources and add them to the corresponding managers, and they
 * must know what they need to do their job.
 *
 * Any renderers added to this manager are freed by calling the freeResources
 * method.
 */
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
