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

		boost::recursive_mutex::scoped_lock lock(m_mutex);
		m_pool_count++;
		return (void *)(m_pool.malloc());
	}

	void * operator new (size_t size, void *p)
	{
		return p;
	}

	void operator delete (void *ptr)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);
		m_pool_count--;
		m_pool.free((T *)ptr);
	}

	void operator delete (void *ptr, T *p)
	{
		int i = 0;
	}

	static size_t get_active_size() { return m_pool_count; }
	static size_t get_next_size() { return m_pool.get_next_size(); }

private:
	static boost::pool<>	m_pool;
	static boost::recursive_mutex	m_mutex;
	static size_t m_pool_count;
};


template <class T>
boost::pool<>	MemoryPoolBase<T>::m_pool(sizeof(T), 32);

template <class T>
boost::recursive_mutex	MemoryPoolBase<T>::m_mutex;

template <class T>
size_t	MemoryPoolBase<T>::m_pool_count;
