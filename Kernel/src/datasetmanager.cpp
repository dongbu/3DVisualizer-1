#include "datasetmanager.h"
#include "logger.h"

void DatasetManager::AddDataset(std::string key, Dataset* data)
{
  if(key.empty() || data == NULL) {
    Logger::getInstance()->error("Dataset::AddDataset invalid parameter.");
    return;
  }

  m_datasetMap[key] = new Dataset(*data);
  std::cout << m_datasetMap[key]->width << ", " << m_datasetMap[key]->height << ", " << m_datasetMap[key]->slices << ", " << m_datasetMap[key]->bytes_elem << std::endl;
  delete data;
}

void DatasetManager::FreeResources()
{
  for(auto it = m_datasetMap.begin(); it != m_datasetMap.end(); it++) {
    delete it->second;
  }

  m_datasetMap.clear();
}
