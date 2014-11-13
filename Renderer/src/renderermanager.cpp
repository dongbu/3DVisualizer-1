#include "renderermanager.h"



bool RendererManager::Init()
{

}

bool RendererManager::Add(std::string key, Renderer* r)
{

}

void RendererManager::SetActive(std::string key)
{

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
