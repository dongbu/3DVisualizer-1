#include "datasetmanager.h"
#include "logger.h"

#define TIXML_USE_STL

#include <tinyxml.h>

using namespace knl;

bool DatasetManager::init(std::string path)
{
  m_dataPath = path;
  TiXmlDocument metafile(path + std::string("metafile.xml"));
  bool load_ok = metafile.LoadFile();

  if(!load_ok) return false;

  TiXmlNode* node = NULL;

  node = metafile.FirstChild("datasets");

  for(TiXmlNode* data_child = node->FirstChild();
      data_child != NULL;
      data_child = data_child->NextSibling()) {

    Dataset* d = new Dataset;
    std::istringstream(data_child->FirstChild("width")->FirstChild()->Value()) >> d->width;
    std::istringstream(data_child->FirstChild("height")->FirstChild()->Value()) >> d->height;
    std::istringstream(data_child->FirstChild("slices")->FirstChild()->Value()) >> d->slices;
    std::istringstream(data_child->FirstChild("bytes_elem")->FirstChild()->Value()) >> d->bytes_elem;

    add(data_child->FirstChild("name")->FirstChild()->Value(), d);
  }

  return true;
}

bool DatasetManager::add(std::string key, Dataset* data)
{
  if(key.empty() || data == NULL) {
    Logger::getInstance()->error("Dataset::Add invalid parameters.");
    return false;
  }

  m_datasetMap[key] = new Dataset(*data);
  return true;
}

bool DatasetManager::setActive(std::string key, GLenum tex_unit)
{
  if(m_activeKey == key) return true;

  std::map<std::string, Dataset*>::iterator it = m_datasetMap.find(key);

  if(it == m_datasetMap.end()) return false;

  Dataset* data = it->second;
  if(!data->isLoaded()) {
    if(!data->load(m_dataPath + it->first + ".raw")) {
      Logger::getInstance()->error("Failed to load dataset " + key);
      return false;
    }
  }
  if(!data->isUploaded()) {
    if(!data->upload()) {
      Logger::getInstance()->error("Failed to upload dataset " + key);
      return false;
    }
  }

  data->bind(tex_unit);
  m_activeKey = key;
  return true;
}

Dataset* DatasetManager::get(std::string key)
{
  assert(!key.empty());
  assert(m_datasetMap.find(key) != m_datasetMap.end());
  return m_datasetMap[key];
}

Dataset* DatasetManager::getCurrent()
{
  return get(m_activeKey);
}

void DatasetManager::freeResources()
{
  for(auto it = m_datasetMap.begin(); it != m_datasetMap.end(); it++) {
    if(it->second->tex_id != 0)
      glDeleteTextures(1, &it->second->tex_id);
    delete it->second;
  }

  m_datasetMap.clear();
}
