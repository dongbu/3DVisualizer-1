#ifndef TFMANAGER_H
#define TFMANAGER_H

#include <map>
#include <string>
#include <vector>

#include "singleton.h"
#include "tfunction.h"

/**
 * @brief The TFManager class
 * This class is responsible for managing the available transfer functions.
 *
 * This class instance holds a map where each TFunction is referenced by a key
 * string. The init method laods a XML metafile containing information about the
 * available functions, such as it's name and number of bytes addressed. After
 * loading the metafile, these functions are created with the parameters read,
 * this way, when the user wants to load one of them, the process knows how much
 * space must be allocated beforehand.
 *
 * To use a transfer function, one must be set as current by using the setActive
 * method. If the transfer function was not loaded before, this method loads it
 * and sends it's data to the OpenGL device.
 *
 * Any transfer functions added to the manager are freed by calling the
 * freeResources method.
 */
class TFManager : public Singleton<TFManager>
{
  friend class Singleton<TFManager>;
public:
  bool init(std::string path);
  bool add(std::string key, TFunction* tf);
  bool setActive(std::string key, GLenum tex_unit = GL_TEXTURE0);
  TFunction* get(std::string key);
  TFunction* getCurrent();
  void freeResources();

  std::vector<std::string> getKeys()
  {
    std::vector<std::string> v;
    for(auto it = m_funcMap.begin(); it != m_funcMap.end(); ++it)
      v.push_back(it->first);
    return v;
  }

  std::string getPath()
  {
    return m_tfPath;
  }

private:
  std::map<std::string, TFunction*> m_funcMap;
  std::string m_activeKey;
  std::string m_tfPath;
};

#endif // TFMANAGER_H
