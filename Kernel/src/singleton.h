#ifndef SINGLETON_H
#define SINGLETON_H

#include <cstdlib>
#include <assert.h>


/**
 * @brief The Singleton class. This class implements the Singleton design
 * pattern.
 *
 * Any class that is designed to be a singleton may inherit from
 * Singleton<MyClass> and declare Singleton<MyClass> as a friend. Also, the
 * Class' constructor and destructor must be declared as protected.
 *
 * Any child class may call the getInstance and destroy methods from the
 * Singleton parent. An example of inheritance and use follows.
 *
 * class MyClass : public Singleton<MyClass>
 * {
 *   friend Singleton<MyClass>;
 * public:
 *   void add(int x);
 *   ...
 * };
 * ...
 * MyClass::getInstance()->add(4);
 */
template <class T>
class Singleton
{
public:
  inline static T* getInstance()
  {
    if (!m_instance)
      m_instance = new T;
    assert(m_instance != NULL);
    return m_instance;
  }

  inline static void destroy()
  {
    if (!m_instance)
      return;
    delete m_instance;
    m_instance = NULL;
  }

protected:
  Singleton(void) {}
  virtual ~Singleton(void) {}

private:
  static T* m_instance;
  Singleton(const Singleton& rhs) {}
  Singleton& operator =(const Singleton& rhs) {}
};

template <class T> T* Singleton<T>::m_instance = NULL;

#endif // SINGLETON_H
