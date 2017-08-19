#pragma once

#include <list>
#include <queue>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition.hpp>

template <class _Ty>
class queue_ts : public std::queue<_Ty>
{
public:
	typedef typename std::queue<_Ty> queue;
	typedef typename std::queue<_Ty>::value_type value_type;
	typedef typename std::queue<_Ty>::size_type size_type;

private:
	boost::recursive_mutex __queue_mutex;
	boost::condition __cond;

	volatile int32_t __i_lock_count;

public:
	queue_ts()
	{
		__i_lock_count = 0;
	}

	~queue_ts() {}

	boost::recursive_mutex &getmutex() { return __queue_mutex; }

	bool empty()
	{
		boost::recursive_mutex::scoped_lock lock(__queue_mutex);
		return queue::empty();
	}

	size_type size()
	{
		boost::recursive_mutex::scoped_lock lock(__queue_mutex);
		return queue::size();
	}

	value_type& front()
	{
		boost::recursive_mutex::scoped_lock lock(__queue_mutex);
		return queue::front();
	}

	// const value_type& front() const
	// {
	// 	boost::recursive_mutex::scoped_lock lock(__queue_mutex);
	// 	return queue::front();
	// }

	value_type& back()
	{
		boost::recursive_mutex::scoped_lock lock(__queue_mutex);
		return queue::back();
	}

	// const value_type& back() const
	// {
	// 	boost::recursive_mutex::scoped_lock lock(__queue_mutex);
	// 	return queue::back();
	// }

	void push(const value_type& _Val)
	{
		boost::recursive_mutex::scoped_lock lock(__queue_mutex);
		queue::push(_Val);
	}

	void pop()
	{
		boost::recursive_mutex::scoped_lock lock(__queue_mutex);
		queue::pop();
	}

	//--------------------------------------------------------------------------
	void wait()
	{
		boost::recursive_mutex::scoped_lock	lock(__queue_mutex);

		while (__i_lock_count == 0)
			__cond.wait(lock);

		--__i_lock_count;
	}

	void signal()
	{
		boost::recursive_mutex::scoped_lock	lock(__queue_mutex);
		// ++__i_lock_count;
		// __cond.notify_one();
		if (++__i_lock_count == 1)
			__cond.notify_one();
	}

	void push_signal(const value_type & _Val)
	{
		boost::recursive_mutex::scoped_lock	lock(__queue_mutex);
		queue::push(_Val);
		// ++__i_lock_count;
		// __cond.notify_one();
		if (++__i_lock_count == 1)
			__cond.notify_one();
	}

	_Ty front_pop()
	{
		boost::recursive_mutex::scoped_lock lock(__queue_mutex);
		_Ty p = queue::front();
		queue::pop();

		return p;
	}

	_Ty wait_front_pop()
	{
		boost::recursive_mutex::scoped_lock	lock(__queue_mutex);

		while (__i_lock_count == 0)
		{
			__cond.wait(lock);
		}
		--__i_lock_count;

		//////////////////////////////////////////////////////////////////////////
		_Ty p = queue::front();
		queue::pop();

		return p;
	}

	void clear()
	{
		boost::recursive_mutex::scoped_lock lock(__queue_mutex);
		while (queue::empty() == false)
		{
			queue::pop();
		}
	}
};
