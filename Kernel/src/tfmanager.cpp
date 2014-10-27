#include "tfmanager.h"
#include "logger.h"

#define TIXML_USE_STL

#include <tinyxml.h>

bool TFManager::Init(std::string path)
{
  m_tfPath = path;

  TiXmlDocument metafile(path + std::string("metafile.xml"));
  bool load_ok = metafile.LoadFile();

  if(!load_ok) return false;

  TiXmlNode* node = NULL;
  //TiXmlElement* elem = NULL;
  //TiXmlElement* data_elem = NULL;

  node = metafile.FirstChild("transfer-functions");
  //data_elem = node->ToElement();

  for(TiXmlNode* data_child = node->FirstChildElement();
      data_child != NULL;
      data_child = data_child->NextSiblingElement()) {

    TFunction* tf = new TFunction;
    std::string type_str = data_child->ToElement()->Attribute("type");

    if(type_str == "alpha") {
      tf->num_channels = 1;
    } else if(type_str == "rgb") {
      tf->num_channels = 3;
    } else if(type_str == "rgba") {
      tf->num_channels = 4;
    }

    std::istringstream(data_child->FirstChild("rows")->FirstChild()->Value()) >> tf->rows;
    std::istringstream(data_child->FirstChild("bytes_elem")->FirstChild()->Value()) >> tf->bytes_elem;

    Add(data_child->FirstChild("name")->FirstChild()->Value(), tf);
  }

  return true;
}

bool TFManager::Add(std::string key, TFunction* tf)
{
  if(key.empty() || tf == NULL) {
    Logger::GetInstance()->error("TFManager::Add invalid parameters.");
    return false;
  }

  m_funcMap[key] = new TFunction(*tf);
  return true;
}

void TFManager::SetActive(std::string key, GLenum tex_unit)
{
  if(m_activeKey == key) return;

  std::map<std::string, TFunction*>::iterator it = m_funcMap.find(key);

  if(it == m_funcMap.end()) return;

  TFunction* tf = it->second;
  if(!tf->IsLoaded()) {
    if(!tf->Load(m_tfPath + it->first + ".raw")) {
      Logger::GetInstance()->error("Failed to load transfer function " + key);
    }
  }
  if(!tf->IsUploaded()) {
    if(!tf->UploadToGPU()) {
      Logger::GetInstance()->error("Failed to upload transfer function " + key);
    }
  }

  tf->bind(tex_unit);
  m_activeKey = key;
}

TFunction* TFManager::Get(std::string key)
{
  assert(!key.empty());
  assert(m_funcMap.find(key) != m_funcMap.end());
  return m_funcMap[key];
}

TFunction* TFManager::GetCurrent()
{
  return Get(m_activeKey);
}

void TFManager::FreeResources()
{
  for(auto it = m_funcMap.begin(); it != m_funcMap.end(); it++) {
    if(it->second->tex_id != 0)
      glDeleteTextures(1, &it->second->tex_id);
    delete it->second;
  }

  m_funcMap.clear();
}
