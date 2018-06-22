#pragma once
#include <functional>

template<typename T>
class Pool
{
	template<typename T>
	friend class Pooling;
public:
	void Release();
	T& Get();
	void Set(const T& _obj);

	static void SetFunc(std::function<void(Pool*)> func)
	{
		release = std::bind(func, std::placeholders::_1);
	}
private:
	T obj;

	static std::function<void(Pool*)> release;
	static void Temp(Pool* zz)
	{
		int a = 10;
	}
	Pool()
	{
	}

	virtual ~Pool()
	{
	}
};

template<typename T>
inline T& Pool<T>::Get()
{
	return obj;
}

template<typename T>
inline void Pool<T>::Set(const T& _obj)
{
	obj = _obj;
}

template<typename T>
inline void Pool<T>::Release()
{
	int a = 10;
	release(this);
}


template<typename T>
std::function<void(Pool<T>*)> Pool<T>::release=std::bind(Pool<T>::Temp, std::placeholders::_1);
