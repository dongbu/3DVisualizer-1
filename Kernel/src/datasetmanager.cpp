#include "datasetmanager.h"
#include "logger.h"

#define TIXML_USE_STL

#include <tinyxml.h>

bool DatasetManager::Init(std::string path)
{
  m_dataPath = path;

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

    AddDataset(data_child->FirstChild("name")->FirstChild()->Value(), d);
  }

  return true;
}

void DatasetManager::AddDataset(std::string key, Dataset* data)
{
  if(key.empty() || data == NULL) {
    Logger::getInstance()->error("Dataset::AddDataset invalid parameter.");
    return;
  }

  m_datasetMap[key] = new Dataset(*data);
}

void DatasetManager::SetActiveDataset(std::string key)
{
  if(m_activeKey == key) return;

  std::map<std::string, Dataset*>::iterator it = m_datasetMap.find(key);

  if(it == m_datasetMap.end()) return;

  Dataset* data = it->second;
  if(!data->IsLoaded()) {
    if(!data->Load(m_dataPath + it->first + ".raw")) {
      Logger::getInstance()->error("Failed to load dataset " + key);
    }
  }
  if(!data->IsUploaded()) {
    if(!data->UploadToGPU()) {
      Logger::getInstance()->error("Failed to upload dataset " + key);
    }
  }
  glBindTexture(GL_TEXTURE_3D, data->tex_id);
  m_activeKey = key;
}

Dataset* DatasetManager::GetDataset(std::string key)
{
  assert(!key.empty());
  assert(m_datasetMap.find(key) != m_datasetMap.end());
  return m_datasetMap[key];
}

void DatasetManager::FreeResources()
{
  for(auto it = m_datasetMap.begin(); it != m_datasetMap.end(); it++) {
    delete it->second;
  }

  m_datasetMap.clear();
}
