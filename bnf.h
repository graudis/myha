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
#include "listen_sessionTcp.h"
#include "rnsocketioserviceTcp.h"
#include "timer_session.h"

#define DEFAULT_IO_WORKER_THREAD_COUNT 4

#define SESSION_HANDLE_CHECKSUM				(0xFF000000)
#define SESSION_INDENTIFY					(0x00000000)
#define CONNECT_SESSION_INDENTIFY			(0x01000000)
#define TIMER_SESSION_INDENTIFY				(0x02000000)
#define LISTEN_SESSION_INDENTIFY			(0x04000000)
// #define DOMAIN_SESSION_INDENTIFY			(0x08000000)
// #define DOMAIN_CONNECT_SESSION_INDENTIFY	(0x0F000000)
// #define DOMAIN_LISTEN_SESSION_INDENTIFY		(0x10000000)


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

	SessionEvent(tINT type, SessionBase* session) : type_(type), session_(session) {}
	SessionEvent(rnSocketIOHandler* handler, void* data) : type_(ON_EVENT), handler_(handler), data_(data) {}

	tINT type_;
	SessionBase* session_;
	rnSocketIOHandler* handler_;
	void* data_;
};

class bnf
{
public:
	friend class SessionBase;
	friend class ListenSessionTcp;
	// friend class ListenSessionDomain;
	friend class rnSocketIOServiceTcp;
	// friend class rnSocketIOServiceDomain;
	friend class TimerSession;

public:
	typedef map_ts<session_handle, SessionBase*> map_session_list_t;
	typedef queue_ts<SessionEvent*> session_queue_t;

	typedef boost::function<void()>	session_process_func;

public:
	bnf();
	~bnf();

	static bnf* instance();

	void Init(void);

	boost::asio::io_service& GetIoService() { return io_service_; }

	session_handle CreateListen(std::string host, int port, int waittimeout, rnSocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0);
	session_handle CreateListen(const char* host, int port, int waittimeout, rnSocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0);
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

	size_t GetSessionCountTcp() { return tcp_session_list_.size(); }
	// size_t GetSessionCountDomain() { return domain_session_list_.size(); }

	SessionEvent* GetSessionFromQueue() { return session_queue_.wait_front_pop(); }
	void PutSessionEvent(tINT type, SessionBase* session) { session_queue_.push_signal(new SessionEvent(type, session)); }

	void PutSessionEvent(rnSocketIOHandler* handler, void* data) { session_queue_.push_signal(new SessionEvent(handler, data)); }

	tINT GetSessionQueueCount() { return session_queue_.size(); }

	SessionBase* GetSessionPointer(session_handle handle);

	void SetSessionProcessFunc(session_process_func func) { session_process_func_ = func; }
	tBOOL ProcessEvent(SessionEvent::SP& session_event);

	void Clear();

	void growSessionBuffer();
	void growConnectSessionBuffer();

	// Domain Service
	// session_handle CreateListenDomain(int port, int waittimeout, rnSocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0);
	// void CloseListenDomain(session_handle handle);
	// session_handle CreateConnectDomain(int port, void* user_data);
	// void growSessionBufferDomain();
	// void growConnectSessionBufferDomain();

private:
	void WorkerThread();
	void RemoveSessionThread();
	void SessionProcessThread();

private:
	boost::ptr_vector<rnSocketIOService> tcp_session_buf_;
	boost::ptr_vector<rnSocketIOService> tcp_connect_session_buf_;
	// boost::ptr_vector<rnSocketIOService> domain_session_buf_;
	// boost::ptr_vector<rnSocketIOService> domain_connect_session_buf_;

	seq_manager_ts<session_handle> timer_session_seq_;
	seq_manager_ts<session_handle> tcp_session_seq_;
	seq_manager_ts<session_handle> tcp_connect_session_seq_;
	seq_manager_ts<session_handle> tcp_listen_session_seq_;
	// seq_manager_ts<session_handle> domain_session_seq_;
	// seq_manager_ts<session_handle> domain_connect_session_seq_;
	// seq_manager_ts<session_handle> domain_listen_session_seq_;

	map_session_list_t timer_session_list_;
	map_session_list_t tcp_session_list_;
	map_session_list_t tcp_connect_session_list_;
	map_session_list_t tcp_listen_session_list_;
	// map_session_list_t domain_session_list_;
	// map_session_list_t domain_connect_session_list_;
	// map_session_list_t domain_listen_session_list_;

	session_queue_t session_queue_;

	boost::thread_group thread_group_;
	boost::asio::io_service io_service_;

	queue_ts<session_handle> remove_session_queue_;

	// 	std::auto_ptr<boost::asio::io_service::work>	io_service_work_;
	boost::shared_ptr<boost::asio::io_service::work>	io_service_work_;

	bool stopped_;

	session_process_func session_process_func_;

	//	boost::asio::io_service::work		io_service_work_;
};
