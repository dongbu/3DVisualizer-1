#ifndef ALPHA_MANAGER_H
#define ALPHA_MANAGER_H

#include <map>
#include <string>
#include <vector>
#include "singleton.h"
#include "dataset.h"

/**
 * @brief The AlphaManager class
 * This class is responsible to manage the available alpha maps.
 *
 * The class instance has two maps. The first map holds every alpha dataset
 * loaded or created by the topological analyzer. This map's key values are
 * strings. The second map holds the alpha,data association, where each alpha
 * dataset is linked to its original dataset.
 *
 * The second map's function is to avoid that an alpha dataset is used to
 * visualize a different volume.
 *
 * The init method loads a XML metafile containing information about the alpha
 * datasets saved, such as its name, original dataset name, dimensions and
 * data type. This allows for a pre-allocation of the resources before actually
 * loading them.
 *
 * To use an alpha dataset to visualize a dataset, one must be set as current by
 * using the setActive method. If the chosen alpha dataset is not loaded, then
 * the setActive method loads it from the file and sends its data to the OpenGL
 * device.
 *
 * Any datasets added to the manager are freed by calling the freeResources
 * method.
 */
class AlphaManager : public Singleton<AlphaManager>
{
  friend class Singleton<AlphaManager>;

public:
  bool init(std::string path);
  bool add(std::string key, std::string data_key, knl::Dataset* alpha_map);
  bool setActive(std::string key, GLenum tex_unit = GL_TEXTURE0);
  bool saveCurrent();
  knl::Dataset* get(std::string key);
  knl::Dataset* getCurrent();
  void freeResources();

  std::string getPath()
  {
    return m_alphaPath;
  }

  std::string getCurrentKey()
  {
    return m_activeKey;
  }

  std::vector<std::string> getKeys()
  {
    std::vector<std::string> v;
    for(auto it = m_alphaMap.begin(); it != m_alphaMap.end(); ++it)
      v.push_back(it->first);
    return v;
  }

private:
  std::map<std::string, knl::Dataset*> m_alphaMap;
  std::map<std::string, std::string> m_alphaDataMap;

  std::string m_activeKey;
  std::string m_alphaPath;
};

#endif // ALPHA_MANAGER_H
