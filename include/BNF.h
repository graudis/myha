#pragma once

#include <boost/version.hpp>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include "map_ts.h"
#include "queue_ts.h"
#include "seq_manager.h"

#include "session_base.h"
#include "ListenSessionTcp.h"
#include "SocketIOServiceTcp.h"
#include "TimerSession.h"

#define DEFAULT_IO_WORKER_THREAD_COUNT 4

#define SESSION_HANDLE_CHECKSUM		(0xFF000000)
#define SESSION_INDENTIFY			(0x00000000)
#define CONNECT_SESSION_INDENTIFY	(0x01000000)
#define TIMER_SESSION_INDENTIFY		(0x02000000)
#define LISTEN_SESSION_INDENTIFY	(0x04000000)

class SessionEvent : public MemoryPoolBase<SessionEvent>
{
public:
	typedef boost::shared_ptr<SessionEvent> SP;

	enum
	{
		ON_EXIT,
		ON_ACCEPT,
		ON_CONNECT,
		ON_CONNECT_FAIL,
		ON_RECEIVE,
		ON_CLOSE,
		ON_TIMER,
		ON_EVENT,
	};

	SessionEvent(int32_t type, SessionBase* session) : type_(type), session_(session) {}
	SessionEvent(SocketIOHandler* handler, void* data) : type_(ON_EVENT), handler_(handler), data_(data) {}

	int32_t type_;
	SessionBase* session_;
	SocketIOHandler* handler_;
	void* data_;
};

class BNF
{
public:
	friend class SessionBase;
	friend class ListenSessionTcp;
	friend class SocketIOServiceTcp;
	friend class TimerSession;

public:
	typedef map_ts<session_handle, SessionBase*> map_session_list_t;
	typedef queue_ts<SessionEvent*> session_queue_t;

	typedef boost::function<void()>	session_process_func;

public:
	BNF();
	~BNF();

	static BNF* instance();

	void Init(void);

	boost::asio::io_service& GetIoService() { return __io_service; }

	session_handle CreateListen(std::string host, int port, int waittimeout, SocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0);
	session_handle CreateListen(const char* host, int port, int waittimeout, SocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0);
	void CloseListen(session_handle handle);

	session_handle CreateConnect(std::string& host, std::string& port, void* user_data);
	session_handle CreateConnect(std::string& host, int port, void* user_data);
	session_handle CreateConnect(const char* host, int port, void* user_data);
				   
	session_handle CreateAsyncConnect(std::string& host, int port, void* user_data);
				   
	session_handle CreateTimer(int msec, void *user_data);
	session_handle CreateTimerPeriod(int start_msec, int period_msec, void *user_data);

	void RemoveSession(session_handle handle);
		 
	void Run(int worker_thread_count = DEFAULT_IO_WORKER_THREAD_COUNT);
	void Stop();

	size_t GetSessionCountTcp() { return __tcp_session_list.size(); }

	SessionEvent* GetSessionFromQueue() { return __session_queue.wait_front_pop(); }
	void PutSessionEvent(int32_t type, SessionBase* session) { __session_queue.push_signal(new SessionEvent(type, session)); }

	void PutSessionEvent(SocketIOHandler* handler, void* data) { __session_queue.push_signal(new SessionEvent(handler, data)); }

	int32_t GetSessionQueueCount() { return __session_queue.size(); }

	SessionBase* GetSessionPointer(session_handle handle);

	void SetSessionProcessFunc(session_process_func func) { __session_process_func = func; }
	bool ProcessEvent(SessionEvent::SP& session_event);

	void Clear();

	void growSessionBuffer();
	void growConnectSessionBuffer();

private:
	void WorkerThread();
	void RemoveSessionThread();
	void SessionProcessThread();

private:
	boost::ptr_vector<SocketIOService> __tcp_session_buf;
	boost::ptr_vector<SocketIOService> __tcp_connect_session_buf;

	seq_manager_ts<session_handle> __timer_session_seq;
	seq_manager_ts<session_handle> __tcp_session_seq;
	seq_manager_ts<session_handle> __tcp_connect_session_seq;
	seq_manager_ts<session_handle> __tcp_listen_session_seq;

	map_session_list_t __timer_session_list;
	map_session_list_t __tcp_session_list;
	map_session_list_t __tcp_connect_session_list;
	map_session_list_t __tcp_listen_session_list;

	session_queue_t __session_queue;

	boost::thread_group __thread_group;
	boost::asio::io_service __io_service;

	queue_ts<session_handle> __remove_session_queue;

	boost::shared_ptr<boost::asio::io_service::work> __io_service_work;

	bool __stopped;

	session_process_func __session_process_func;
};
