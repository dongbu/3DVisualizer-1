#ifndef DATASETMANAGER_H
#define DATASETMANAGER_H

#include <map>
#include <vector>
#include <string>

#include "singleton.h"
#include "dataset.h"

/**
 * @brief The DatasetManager class
 * This class is responsible to manage the avaliable datasets.
 *
 * The class instance holds a map, where each Dataset is referenced by a key
 * string. The init method loads a XML metafile containing information about the
 * available datasets, such as its name, dimensions and number of bytes of each
 * element. These Datasets are then created and given this information. This
 * process facilitates the loading of the dataset from the disk.
 *
 * To visualize a dataset, one must be set as current by using the setActive
 * method. If the dataset is not loaded yet, this method loads and sends its
 * data to the OpenGL device before finishing.
 *
 * Any datasets added to the manager are freed by calling the freeResources
 * method.
 */
class DatasetManager : public Singleton<DatasetManager>
{
  friend class Singleton<DatasetManager>;
  
public:
  bool init(std::string path);
  bool add(std::string key, knl::Dataset* data);
  bool setActive(std::string key, GLenum tex_unit = GL_TEXTURE0);
  knl::Dataset* get(std::string key);
  knl::Dataset* getCurrent();
  void freeResources();

  std::string getCurrentKey()
  {
    return m_activeKey;
  }

  std::vector<std::string> getKeys()
  {
    std::vector<std::string> v;
    for(auto it = m_datasetMap.begin(); it != m_datasetMap.end(); ++it)
      v.push_back(it->first);
    return v;
  }

  std::string getPath()
  {
    return m_dataPath;
  }

private:
  std::map<std::string, knl::Dataset*> m_datasetMap;
  std::string m_activeKey;
  std::string m_dataPath;
};

#endif // DATASETMANAGER_H
