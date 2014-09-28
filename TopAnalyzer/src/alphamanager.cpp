#include "alphamanager.h"
#include "logger.h"

#define TIXML_USE_STL

#include <tinyxml.h>

using namespace knl;

bool AlphaManager::Init(std::string path)
{
  m_vtbPath = path;

  TiXmlDocument metafile(path + std::string("metafile.xml"));
  bool load_ok = metafile.LoadFile();

  if(!load_ok) return false;

  TiXmlNode* node = NULL;
  TiXmlElement* elem = NULL;
  TiXmlElement* data_elem = NULL;

  node = metafile.FirstChild("datasets");
  data_elem = node->ToElement();

  for(TiXmlNode* data_child = node->FirstChild(); data_child != NULL; data_child = data_child->NextSibling()) {
    Dataset* d = new Dataset;
    std::istringstream(data_child->FirstChild("width")->FirstChild()->Value()) >> d->width;
    std::istringstream(data_child->FirstChild("height")->FirstChild()->Value()) >> d->height;
    std::istringstream(data_child->FirstChild("slices")->FirstChild()->Value()) >> d->slices;
    std::istringstream(data_child->FirstChild("bytes_elem")->FirstChild()->Value()) >> d->bytes_elem;

    Add(data_child->FirstChild("name")->FirstChild()->Value(), d);
  }

  return true;
}

bool AlphaManager::Add(std::string key, Dataset* data)
{
  if(key.empty() || data == NULL) {
    Logger::getInstance()->error("Dataset::Add invalid parameters.");
    return false;
  }

  m_vtbMap[key] = new Dataset(*data);
  return true;
}

void AlphaManager::SetActive(std::string key, GLenum tex_unit)
{
  if(m_activeKey == key) return;

  std::map<std::string, Dataset*>::iterator it = m_vtbMap.find(key);

  if(it == m_vtbMap.end()) return;

  Dataset* data = it->second;
  if(!data->IsLoaded()) {
    if(!data->Load(m_vtbPath + it->first + ".raw")) {
      Logger::getInstance()->error("Failed to load dataset " + key);
    }
  }
  if(!data->IsUploaded()) {
    if(!data->UploadToGPU()) {
      Logger::getInstance()->error("Failed to upload dataset " + key);
    }
  }

  data->bind(tex_unit);
  m_activeKey = key;
}

Dataset* AlphaManager::Get(std::string key)
{
  assert(!key.empty());
  assert(m_vtbMap.find(key) != m_vtbMap.end());
  return m_vtbMap[key];
}

Dataset* AlphaManager::GetCurrent()
{
  return Get(m_activeKey);
}

void AlphaManager::FreeResources()
{
  for(auto it = m_vtbMap.begin(); it != m_vtbMap.end(); it++) {
    delete it->second;
  }

  m_vtbMap.clear();
}
