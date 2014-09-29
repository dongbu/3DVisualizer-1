#ifndef SINGLETON_H
#define SINGLETON_H

#include <cstdlib>
#include <assert.h>

template <class T>
class Singleton {
public:
	inline static T* GetInstance()
	{
		if (!m_instance)
			m_instance = new T;
		assert(m_instance != NULL);
		return m_instance;
	}

	inline static void DestroyInstance()
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
