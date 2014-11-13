#include "renderermanager.h"
#include "logger.h"

bool RendererManager::Init()
{
  return false;
}

bool RendererManager::Add(std::string key, Renderer* r)
{
  if(key.empty() || r == NULL) {
    Logger::GetInstance()->error("RendererManager::Add invalid parameters");
    return false;
  }

  m_rendererMap[key] = r;
  return false;
}

void RendererManager::SetActive(std::string key)
{
  if(key.empty() || m_activeKey == key) return;
}

Renderer* RendererManager::Get(std::string key)
{
  return NULL;
}

Renderer* RendererManager::GetCurrent()
{
  return Get(m_activeKey);
}

void RendererManager::FreeResources()
{

}
