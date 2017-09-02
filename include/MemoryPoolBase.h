#pragma once

#include <assert.h>
#include <boost/pool/pool.hpp>
#include <boost/thread/recursive_mutex.hpp>

template <class T>
class MemoryPoolBase
{
public:
	MemoryPoolBase(void) {}
	virtual ~MemoryPoolBase(void) {}

	void * operator new (size_t size)
	{
		(void)size;

		boost::recursive_mutex::scoped_lock lock(__mutex);
		__pool_count++;
		return (void *)(__pool.malloc());
	}

	void * operator new (size_t size, void *p)
	{
		return p;
	}

	void operator delete (void *ptr)
	{
		boost::recursive_mutex::scoped_lock lock(__mutex);
		__pool_count--;
		__pool.free((T *)ptr);
	}

	void operator delete (void *ptr, T *p)
	{
		// int i = 0;
	}

	static size_t get_active_size() { return __pool_count; }
	static size_t get_next_size() { return __pool.get_next_size(); }

private:
	static boost::pool<> __pool;
	static boost::recursive_mutex __mutex;
	static size_t __pool_count;
};

template <class T>
boost::pool<>	MemoryPoolBase<T>::__pool(sizeof(T), 32);

template <class T>
boost::recursive_mutex	MemoryPoolBase<T>::__mutex;

template <class T>
size_t	MemoryPoolBase<T>::__pool_count;
