#pragma once

template <class T>
class Singleton
{
private:
	static T* instance;
public:
	static T* GetInstance();
	static void FreeInstance();
	Singleton(void);
	virtual ~Singleton(void);
};


template <class T>
T* Singleton<T>::instance = 0;

template <class T>
Singleton<T>::Singleton(void)
{	
	instance = (T*) this;
}

template <class T>
Singleton<T>::~Singleton(void)
{
	instance = 0;
}

template <class T>
void Singleton<T>::FreeInstance(void)
{
	if ( instance ) 
		delete instance;
	instance = 0;
}

template <class T>
T* Singleton<T>::GetInstance(void)
{
	if ( ! instance )
		new T;
	return instance;
}