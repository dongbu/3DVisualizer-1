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

  std::map<std::string, Renderer*>::iterator it = m_rendererMap.find(key);

  if(it == m_rendererMap.end()) return;

  m_activeKey = key;

  //TODO:SetActive here somehow.
}

Renderer* RendererManager::Get(std::string key)
{
  assert(!key.empty());
  assert(m_rendererMap.find(key) != m_rendererMap.end());
  return m_rendererMap[key];
}

Renderer* RendererManager::GetCurrent()
{
  return Get(m_activeKey);
}

void RendererManager::FreeResources()
{
  for(auto it = m_rendererMap.begin(); it != m_rendererMap.end(); it++) {
    delete it->second;
  }

  m_rendererMap.clear();
}
