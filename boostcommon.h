#pragma once

#include <deque>
#include <stack>
#include <queue>
#include <vector>
#include <list>
#include <map>
#include <set>

#define BOOST_DEFAULT_ALLOC_SIZE		(32)

template <typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_stack : public std::stack<_Ty>
{
};

template <typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_queue : public std::queue<_Ty>
{
};

template <typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_deque : public std::deque<_Ty>
{
};

template<typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_vector : public std::vector<_Ty>
{
};

template<typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_list : public std::list<_Ty>
{
};

template<typename _Kty, typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_map : public std::map<_Kty, _Ty>
{
};

template<typename _Kty, typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_multimap : public std::multimap<_Kty, _Ty>
{
};

template<typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_set : public std::set<_Ty>
{
};


//////////////////////////////////////////////////////////////////////////
// thread safety
#include <boost/thread/recursive_mutex.hpp>

class boost_pool_thread_base
{
public:
	boost::recursive_mutex&		getMutex() { return mutex_; }

private:
	boost::recursive_mutex		mutex_;
};

template <typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_stack_ts : public boost_pool_thread_base, public std::stack<_Ty>
{
};

template <typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_queue_ts : public boost_pool_thread_base, public std::queue<_Ty>
{
};

template <typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_deque_ts : public boost_pool_thread_base, public std::deque<_Ty>
{
};

template<typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_vector_ts : public boost_pool_thread_base, public std::vector<_Ty>
{
};

template<typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_list_ts : public boost_pool_thread_base, public std::list<_Ty>
{
};

template<typename _Kty, typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_map_ts : public boost_pool_thread_base, public std::map<_Kty, _Ty>
{
};

template<typename _Kty, typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_multimap_ts : public boost_pool_thread_base, public std::multimap<_Kty, _Ty>
{
};

template<typename _Ty, int size = BOOST_DEFAULT_ALLOC_SIZE>
class boost_pool_set_ts : public boost_pool_thread_base, public std::set<_Ty>
{
};
