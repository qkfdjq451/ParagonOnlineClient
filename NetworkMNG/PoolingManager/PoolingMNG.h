#pragma once
#include <queue>
#include <list>
#include <map>
#include <functional>
#include "Pool.h"

template<typename T>
class Pooling
{
public:
	Pool<T>* GetObject()
	{
		auto retval = pools.front();
		pools.pop();
		return retval;
	}

private:
	Pooling(int count) :count(count)
	{
		Pool<T>::SetFunc([this](Pool<T>* pThis)	{pools.push(pThis);	});

		for (int i = 0; i < count; i++)
		{
			pools.push(new Pool<T>());
		}
	}

	~Pooling()
	{
		while (!pools.empty())
		{
			delete pools.front();
			pools.pop();
		}
	}

	int count;
	std::queue<Pool<T>*> pools;

	static Pooling* inst;
public:
	static Pooling* Create(int count)
	{
		if (inst == nullptr)
		{
			inst = new Pooling(count);
		}
		return inst;
	}
	static Pooling* Get()
	{
		return inst;
	}
	static void Destroy()
	{
		if (inst != nullptr)
		{
			delete inst;
			inst = nullptr;
		}
	}
};

template<typename T>
Pooling<T>* Pooling<T>::inst = nullptr;

