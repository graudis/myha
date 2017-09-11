#include <csignal>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include "BNF.h"
#include "LogSystem.h"


BNF::BNF() : __io_service_work(new boost::asio::io_service::work(__io_service))
{
	__session_process_func = NULL;
	__stopped = false;
}

BNF::~BNF()
{
}

BNF* BNF::instance()
{
	static BNF __instance;
	return &__instance;
}

void BNF::Init(void)
{
	for (int i = 0; i <= SEQ_MANAGER_GROW_INDEX_COUNT; i++)
	{
		SocketIOServiceTcp* p1 = new SocketIOServiceTcp(__io_service);
		__tcp_session_buf.push_back(p1);
	}

	for (int i = 0; i <= SEQ_MANAGER_GROW_INDEX_COUNT; i++)
	{
		SocketIOServiceTcp* p1 = new SocketIOServiceTcp(__io_service);
		__tcp_connect_session_buf.push_back(p1);
	}

	__timer_session_seq.init(TIMER_SESSION_INDENTIFY, TIMER_SESSION_INDENTIFY + SEQ_MANAGER_GROW_INDEX_COUNT);
	__tcp_session_seq.init(1, SEQ_MANAGER_GROW_INDEX_COUNT);
	__tcp_connect_session_seq.init(CONNECT_SESSION_INDENTIFY, CONNECT_SESSION_INDENTIFY + SEQ_MANAGER_GROW_INDEX_COUNT);
	__tcp_listen_session_seq.init(LISTEN_SESSION_INDENTIFY, LISTEN_SESSION_INDENTIFY + SEQ_MANAGER_GROW_INDEX_COUNT);

#ifndef WIN32
	signal(SIGPIPE, SIG_IGN);
#endif
}

void BNF::WorkerThread()
{
	boost::system::error_code ec;
	__io_service.run(ec);
}

void BNF::RemoveSessionThread()
{
	map_session_list_t::iterator it;

	while (1)
	{
		session_handle handle = __remove_session_queue.wait_front_pop();
		if (handle == SessionBase::INVALID_SESSION_HANDLE)
			break;

		switch (handle & SESSION_HANDLE_CHECKSUM)
		{
		case SESSION_INDENTIFY:
		{
			it = __tcp_session_list.find(handle);
			if (it != __tcp_session_list.end())
			{
				if (it->second->isCloseError() == false)
				{
					__tcp_session_list.erase(it);
					BNF::instance()->__tcp_session_seq.push(handle);
				}
			}
		}
		break;
		case CONNECT_SESSION_INDENTIFY:
		{
			it = __tcp_connect_session_list.find(handle);
			if (it != __tcp_connect_session_list.end())
			{
				if (it->second->isCloseError() == false)
				{
					__tcp_connect_session_list.erase(it);
					BNF::instance()->__tcp_connect_session_seq.push(handle);
				}
			}
		}
		break;
		case TIMER_SESSION_INDENTIFY:
		{
			it = __timer_session_list.find(handle);
			if (it != __timer_session_list.end())
			{
				((TimerSession*)it->second)->Close();
				__timer_session_list.erase(it);
			}
		}
		break;
		case LISTEN_SESSION_INDENTIFY:
		{
			it = __tcp_listen_session_list.find(handle);
			if (it != __tcp_listen_session_list.end())
			{
				((ListenSession *)it->second)->Close();
				delete it->second;
				__tcp_listen_session_list.erase(it);
			}
		}
		break;
		}
	}
}

void BNF::SessionProcessThread()
{
	if (__session_process_func != NULL)
	{
		__session_process_func();
		return;
	}

	while (1)
	{
		SessionEvent::SP session_event(BNF::instance()->GetSessionFromQueue());

		if (ProcessEvent(session_event) == false)
			return;
	}
}

bool BNF::ProcessEvent(SessionEvent::SP& session_event)
{
	switch (session_event->type_)
	{
	case SessionEvent::ON_EXIT:
		return false;
	case SessionEvent::ON_ACCEPT:
	case SessionEvent::ON_RECEIVE:
	case SessionEvent::ON_CLOSE:
	case SessionEvent::ON_TIMER:
	{
		SocketIOHandler* handler = (SocketIOHandler*)(session_event->session_->GetUserData());
		if (handler != NULL)
			handler->operate((SocketIOService*)session_event->session_);
	}
	break;
	case SessionEvent::ON_CONNECT:
	{
		SocketIOHandler* handler = (SocketIOHandler*)(session_event->session_->GetUserData());
		if (handler != NULL)
			handler->onConnect((SocketIOService*)session_event->session_);
	}
	break;
	case SessionEvent::ON_CONNECT_FAIL:
	{
		SocketIOHandler* handler = (SocketIOHandler*)(session_event->session_->GetUserData());
		if (handler != NULL)
			handler->onConnectFail((SocketIOService*)session_event->session_);
	}
	break;
	case SessionEvent::ON_EVENT:
	{
		if (session_event->handler_ != NULL)
			session_event->handler_->onEvent(session_event->data_);
	}
	break;
	}

	return true;
}

void BNF::Run(int worker_thread_count)
{
	// session process thread
	boost::thread session_process(boost::bind(&BNF::SessionProcessThread, this));

	// remove session thread
	boost::thread remove_session(boost::bind(&BNF::RemoveSessionThread, this));

	// i/o worker thread
	for (int i = 0; i < worker_thread_count; i++)
	{
		__thread_group.create_thread(boost::bind(&BNF::WorkerThread, this));
		LOG_INFO("bnf - start worker therad.");
	}

	LOG_INFO("bnf - running.");

	__thread_group.join_all();

	LOG_INFO("bnf - worker thread end.");

	__io_service.stop();

	// session process 종료 Run
	PutSessionEvent(SessionEvent::ON_EXIT, NULL);

	session_process.join();

	LOG_INFO("bnf - session process thread end.");

	// remove session 종료
	__remove_session_queue.push_signal(SessionBase::INVALID_SESSION_HANDLE);
	remove_session.join();

	LOG_INFO("bnf - remove session thread end.");
}

session_handle BNF::CreateListen(std::string host, int port, int waittimeout, SocketIOHandler* func, size_t receive_buffer_size, size_t send_buffer_size)
{
	if (__stopped == true)
		return SessionBase::INVALID_SESSION_HANDLE;

	ListenSessionTcp *pListenSession = new ListenSessionTcp(__io_service, waittimeout);

	if (pListenSession->Run(host, port, func, receive_buffer_size, send_buffer_size) == false)
	{
		delete pListenSession;
		return SessionBase::INVALID_SESSION_HANDLE;
	}

	__tcp_listen_session_list.insert(pListenSession->GetHandle(), pListenSession);

	return pListenSession->GetHandle();
}

session_handle BNF::CreateListen(const char* host, int port, int waittimeout, SocketIOHandler* func, size_t receive_buffer_size, size_t send_buffer_size)
{
	std::string host_str = (host != NULL) ? host : "0.0.0.0";
	return CreateListen(host_str, port, waittimeout, func, receive_buffer_size, send_buffer_size);
}

void BNF::RemoveSession(session_handle handle)
{
	__remove_session_queue.push_signal(handle);
}

void BNF::CloseListen(session_handle handle)
{
	map_session_list_t::iterator it = __tcp_listen_session_list.find(handle);
	if (it == __tcp_listen_session_list.end())
		return;

	delete it->second;
	__tcp_listen_session_list.erase(it);
}

void BNF::Stop()
{
	__stopped = true;

	// listen 종료
	{
		boost::recursive_mutex::scoped_lock lock1(__tcp_listen_session_list.getmutex());
		BOOST_FOREACH(map_session_list_t::value_type p, __tcp_listen_session_list)
		{
			((ListenSession *)p.second)->Close();
		}
	}

	{
		// session
		boost::recursive_mutex::scoped_lock lock1(__tcp_session_list.getmutex());
		BOOST_FOREACH(map_session_list_t::value_type p, __tcp_session_list)
		{
			((SocketIOService *)p.second)->Close("server stop.");
		}
	}

	{
		// connect session
		boost::recursive_mutex::scoped_lock lock1(__tcp_connect_session_list.getmutex());
		BOOST_FOREACH(map_session_list_t::value_type p, __tcp_connect_session_list)
		{
			((SocketIOService *)p.second)->Close("server stop.");
		}
	}

	{
		// timer session
		boost::recursive_mutex::scoped_lock lock(__timer_session_list.getmutex());
		BOOST_FOREACH(map_session_list_t::value_type p, __timer_session_list)
		{
			((TimerSession*)p.second)->Close();
		}
	}

	// io service thread 종료
	__io_service_work.reset();
}

void BNF::Clear()
{
	static bool _exit_flag = false;

	if (_exit_flag)
		return;

	_exit_flag = true;

	__tcp_session_list.clear();
	__tcp_connect_session_list.clear();
	__tcp_listen_session_list.clear();
	__timer_session_list.clear();

	__tcp_session_buf.clear();
	__tcp_connect_session_buf.clear();
}

session_handle BNF::CreateConnect(std::string& host, std::string& port, void* user_data)
{
	if (__stopped == true)
		return SessionBase::INVALID_SESSION_HANDLE;

	session_handle shandle;
	if (__tcp_connect_session_seq.pop(shandle) == false)
		growConnectSessionBuffer();

	SocketIOServiceTcp* pSession = (SocketIOServiceTcp*)&__tcp_connect_session_buf[shandle - CONNECT_SESSION_INDENTIFY];
	pSession->Open(shandle);
	boost::asio::ip::tcp::socket& socket = pSession->Socket();

	boost::asio::ip::tcp::resolver resolver(__io_service);
	boost::asio::ip::tcp::resolver::query query(host, port);

	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::ip::tcp::resolver::iterator end;

	boost::system::error_code ec = boost::asio::error::host_not_found;

	while (ec && endpoint_iterator != end)
	{
		socket.close();
		socket.connect(*endpoint_iterator++, ec);
	}

	if (ec)
	{
		socket.close();
		__tcp_connect_session_seq.push(shandle);
		pSession->_error_message = ec.message();
		return SessionBase::INVALID_SESSION_HANDLE;
	}

	__tcp_connect_session_list.insert(pSession->GetHandle(), pSession);

	pSession->SetUserData(user_data);
	pSession->SetType(SessionBase::CONNECT_SESSION);
	pSession->_getIp();
	pSession->IncRefCount();

	LOG_INFO("bnf [%s] CreateConnect - connected. fd: %d, handle: %d", pSession->ip().c_str(), pSession->Socket().native(), pSession->GetHandle());

	pSession->Run();

	return pSession->GetHandle();
}

session_handle BNF::CreateConnect(std::string& host, int port, void* user_data)
{
	std::string port_str = (boost::format("%d") % port).str();
	return CreateConnect(host, port_str, user_data);
}

session_handle BNF::CreateConnect(const char* host, int port, void* user_data)
{
	std::string host_str = host;
	std::string port_str = (boost::format("%d") % port).str();
	return CreateConnect(host_str, port_str, user_data);
}

session_handle BNF::CreateAsyncConnect(std::string& host, int port, void* user_data)
{
	if (__stopped == true)
		return SessionBase::INVALID_SESSION_HANDLE;

	session_handle shandle;
	if (__tcp_connect_session_seq.pop(shandle) == false)
	{
		growConnectSessionBuffer();
	}

	SocketIOService *pSession = &__tcp_connect_session_buf[shandle - CONNECT_SESSION_INDENTIFY];
	pSession->Open(shandle);

	pSession->SetUserData(user_data);
	pSession->SetType(SessionBase::CONNECT_SESSION);
	pSession->IncRefCount();

	__tcp_connect_session_list.insert(pSession->GetHandle(), pSession);

	if (pSession->AsyncConnect(host, port) == false)
	{
		__tcp_connect_session_list.erase(pSession->GetHandle());

		__tcp_connect_session_seq.push(shandle);
		return SessionBase::INVALID_SESSION_HANDLE;
	}

	return pSession->GetHandle();
}

SessionBase* BNF::GetSessionPointer(session_handle handle)
{
	map_session_list_t::iterator it;

	switch (handle & SESSION_HANDLE_CHECKSUM)
	{
	case SESSION_INDENTIFY:
	{
		it = __tcp_session_list.find(handle);
		if (it != __tcp_session_list.end())
		{
			return it->second;
		}
	}
	break;
	case CONNECT_SESSION_INDENTIFY:
	{
		it = __tcp_connect_session_list.find(handle);
		if (it != __tcp_connect_session_list.end())
		{
			return it->second;
		}
	}
	break;
	case TIMER_SESSION_INDENTIFY:
	{
		it = __timer_session_list.find(handle);
		if (it != __timer_session_list.end())
		{
			return it->second;
		}
	}
	break;
	case LISTEN_SESSION_INDENTIFY:
	{
		it = __tcp_listen_session_list.find(handle);
		if (it != __tcp_listen_session_list.end())
		{
			return it->second;
		}
	}
	break;
	}

	return NULL;
}

session_handle BNF::CreateTimer(int msec, void *user_data)
{
	if (__stopped == true)
		return SessionBase::INVALID_SESSION_HANDLE;

	SessionBase* pSession = new TimerSession(__io_service, msec, msec);
	pSession->SetUserData(user_data);

	__timer_session_list.insert(pSession->GetHandle(), pSession);

	return pSession->GetHandle();
}

session_handle BNF::CreateTimerPeriod(int start_msec, int period_msec, void *user_data)
{
	if (__stopped == true)
		return SessionBase::INVALID_SESSION_HANDLE;

	SessionBase* pSession = new TimerSession(__io_service, start_msec, period_msec);
	pSession->SetUserData(user_data);

	__timer_session_list.insert(pSession->GetHandle(), pSession);

	return pSession->GetHandle();
}

void BNF::growSessionBuffer()
{
	for (int32_t i = 0; i < SEQ_MANAGER_GROW_INDEX_COUNT; ++i)
	{
		SocketIOServiceTcp* p = new SocketIOServiceTcp(__io_service);
		__tcp_session_buf.push_back(p);
	}
}

void BNF::growConnectSessionBuffer()
{
	for (int32_t i = 0; i < SEQ_MANAGER_GROW_INDEX_COUNT; ++i)
	{
		SocketIOServiceTcp* p = new SocketIOServiceTcp(__io_service);
		__tcp_connect_session_buf.push_back(p);
	}
}
