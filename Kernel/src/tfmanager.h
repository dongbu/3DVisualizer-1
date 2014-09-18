#ifndef TFMANAGER_H
#define TFMANAGER_H

#include <map>
#include <string>
#include "singleton.h"
#include "tfunction.h"

class TFManager : public Singleton<TFManager>
{
  friend class Singleton<TFManager>;
public:
  bool Init(std::string path);
  bool Add(std::string key, TFunction* tf);
  void SetActive(std::string key);
  TFunction* Get(std::string key);
  TFunction* GetCurrent();
  void FreeResources();

private:
  std::map<std::string, TFunction*> m_funcMap;
  std::string m_activeKey;
  std::string m_tfPath;
};

#endif // TFMANAGER_H