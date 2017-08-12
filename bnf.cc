#include <csignal>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include "bnf.h"
#include "logsystem.h"


bnf::bnf() : io_service_work_( new boost::asio::io_service::work( io_service_ ) )
{
	session_process_func_ = NULL;
	stopped_ = false;
}

bnf::~bnf()
{
}

bnf* bnf::instance()
{
	static bnf __instance;
	return &__instance;
}

void bnf::Init(void)
{
	for (int i = 0; i <= SEQ_MANAGER_GROW_INDEX_COUNT; i++)
	{
		rnSocketIOServiceTcp* p1 = new rnSocketIOServiceTcp(io_service_);
		tcp_session_buf_.push_back(p1);
		// rnSocketIOServiceDomain* p2 = new rnSocketIOServiceDomain(io_service_);
		// domain_session_buf_.push_back(p2);
	}

	for (int i = 0; i <= SEQ_MANAGER_GROW_INDEX_COUNT; i++)
	{
		rnSocketIOServiceTcp* p1 = new rnSocketIOServiceTcp(io_service_);
		tcp_connect_session_buf_.push_back(p1);
		// rnSocketIOServiceDomain* p2 = new rnSocketIOServiceDomain(io_service_);
		// domain_connect_session_buf_.push_back(p2);
	}

	timer_session_seq_.init(TIMER_SESSION_INDENTIFY, TIMER_SESSION_INDENTIFY + SEQ_MANAGER_GROW_INDEX_COUNT);
	tcp_session_seq_.init(1, SEQ_MANAGER_GROW_INDEX_COUNT);
	tcp_connect_session_seq_.init(CONNECT_SESSION_INDENTIFY, CONNECT_SESSION_INDENTIFY + SEQ_MANAGER_GROW_INDEX_COUNT);
	tcp_listen_session_seq_.init(LISTEN_SESSION_INDENTIFY, LISTEN_SESSION_INDENTIFY + SEQ_MANAGER_GROW_INDEX_COUNT);
	// domain_connect_session_seq_.init(DOMAIN_CONNECT_SESSION_INDENTIFY, DOMAIN_CONNECT_SESSION_INDENTIFY + SEQ_MANAGER_GROW_INDEX_COUNT);
	// domain_session_seq_.init(DOMAIN_SESSION_INDENTIFY, DOMAIN_SESSION_INDENTIFY + SEQ_MANAGER_GROW_INDEX_COUNT);
	// domain_listen_session_seq_.init(DOMAIN_LISTEN_SESSION_INDENTIFY, DOMAIN_LISTEN_SESSION_INDENTIFY + SEQ_MANAGER_GROW_INDEX_COUNT);


#ifndef WIN32
	signal( SIGPIPE, SIG_IGN );
#endif
}

void bnf::WorkerThread()
{
	boost::system::error_code ec;
	io_service_.run( ec );
}

void bnf::RemoveSessionThread()
{
	map_session_list_t::iterator it;

	while (1)
	{
		session_handle handle = remove_session_queue_.wait_front_pop();
		if (handle == SessionBase::INVALID_SESSION_HANDLE)
		{
			break;
		}

		switch (handle & SESSION_HANDLE_CHECKSUM)
		{
		case SESSION_INDENTIFY:
			it = tcp_session_list_.find(handle);
			if (it != tcp_session_list_.end())
			{
				if( it->second->isCloseError() == false )
				{
					tcp_session_list_.erase(it);
					bnf::instance()->tcp_session_seq_.push( handle );
				}
			}
			break;

		case CONNECT_SESSION_INDENTIFY:
			it = tcp_connect_session_list_.find(handle);
			if (it != tcp_connect_session_list_.end())
			{
				if( it->second->isCloseError() == false )
				{
					tcp_connect_session_list_.erase(it);
					bnf::instance()->tcp_connect_session_seq_.push( handle );
				}
			}
			break;

		case TIMER_SESSION_INDENTIFY:
			it = timer_session_list_.find(handle);
			if (it != timer_session_list_.end())
			{
				((TimerSession*)it->second)->Close();
				timer_session_list_.erase(it);
			}
			break;

		case LISTEN_SESSION_INDENTIFY:
			it = tcp_listen_session_list_.find(handle);
			if (it != tcp_listen_session_list_.end())
			{
				((ListenSession *)it->second)->Close();
				delete it->second;
				tcp_listen_session_list_.erase(it);
			}
			break;

		// case DOMAIN_SESSION_INDENTIFY:
		// 	it = domain_session_list_.find(handle);
		// 	if (it != domain_session_list_.end())
		// 	{
		// 		if( it->second->isCloseError() == false )
		// 		{
		// 			domain_session_list_.erase(it);
		// 			bnf::instance()->domain_session_seq_.push( handle );
		// 		}
		// 	}
		// 	break;

		// case DOMAIN_CONNECT_SESSION_INDENTIFY:
		// 	it = domain_connect_session_list_.find(handle);
		// 	if (it != domain_connect_session_list_.end())
		// 	{
		// 		if( it->second->isCloseError() == false )
		// 		{
		// 			domain_connect_session_list_.erase(it);
		// 			bnf::instance()->domain_connect_session_seq_.push( handle );
		// 		}
		// 	}
		// 	break;

		// case DOMAIN_LISTEN_SESSION_INDENTIFY:
		// 	it = domain_listen_session_list_.find(handle);
		// 	if (it != domain_listen_session_list_.end())
		// 	{
		// 		((ListenSession *)it->second)->Close();
		// 		delete it->second;
		// 		domain_listen_session_list_.erase(it);
		// 	}
		// 	break;
		}
	}
}

void bnf::SessionProcessThread()
{
	if( session_process_func_ != NULL )
	{
		session_process_func_();
	}
	else
	{
		while( 1 )
		{
			SessionEvent::SP session_event( bnf::instance()->GetSessionFromQueue() );

			if( ProcessEvent( session_event ) == cFALSE )
				return;
		}
	}
}

tBOOL bnf::ProcessEvent( SessionEvent::SP& session_event )
{
	switch( session_event->type_ )
	{
	case SessionEvent::ON_EXIT:
		return cFALSE;
		break;
	case SessionEvent::ON_ACCEPT:
	case SessionEvent::ON_RECEIVE:
	case SessionEvent::ON_CLOSE:
	case SessionEvent::ON_TIMER:
		{
			rnSocketIOHandler* handler = (rnSocketIOHandler*)(session_event->session_->GetUserData());
			if( handler != NULL )
				handler->operate( (rnSocketIOService*)session_event->session_ );
		}
		break;
	case SessionEvent::ON_CONNECT:
		{
			rnSocketIOHandler* handler = (rnSocketIOHandler*)(session_event->session_->GetUserData());
			if( handler != NULL )
				handler->onConnect( (rnSocketIOService*)session_event->session_ );
		}
		break;
	case SessionEvent::ON_CONNECT_FAIL:
		{
			rnSocketIOHandler* handler = (rnSocketIOHandler*)(session_event->session_->GetUserData());
			if( handler != NULL )
				handler->onConnectFail( (rnSocketIOService*)session_event->session_ );
		}
		break;
	case SessionEvent::ON_EVENT:
		{
			if( session_event->handler_ != NULL )
				session_event->handler_->onEvent( session_event->data_ );
		}
		break;
	}

	return cTRUE;
}

void bnf::Run( int worker_thread_count )
{
	// session process thread
	boost::thread session_process(boost::bind(&bnf::SessionProcessThread, this));

	// remove session thread
	boost::thread remove_session(boost::bind(&bnf::RemoveSessionThread, this));

	// i/o worker thread
	for( int i = 0; i < worker_thread_count; i++ )
	{
		thread_group_.create_thread(boost::bind(&bnf::WorkerThread, this));
		LOG_INFO( "bnf - start worker therad." );
	}

	LOG_INFO( "bnf - running." );

	thread_group_.join_all();

	LOG_INFO( "bnf - worker thread end." );

	io_service_.stop();

	// session process 종료Run
	PutSessionEvent( SessionEvent::ON_EXIT, NULL );

	session_process.join();

	LOG_INFO( "bnf - session process thread end." );

	// remove session 종료
	remove_session_queue_.push_signal(SessionBase::INVALID_SESSION_HANDLE);
	remove_session.join();

	LOG_INFO( "bnf - remove session thread end." );
}

session_handle bnf::CreateListen( std::string host, int port, int waittimeout,
								 rnSocketIOHandler* func, size_t receive_buffer_size, size_t send_buffer_size)
{
	if( stopped_ == true )
		return SessionBase::INVALID_SESSION_HANDLE;

	ListenSessionTcp *pListenSession = new ListenSessionTcp(io_service_, waittimeout);

	if( false == pListenSession->Run(host, port, func, receive_buffer_size, send_buffer_size) )
	{
		delete pListenSession;
		return SessionBase::INVALID_SESSION_HANDLE;
	}

	tcp_listen_session_list_.insert(pListenSession->GetHandle(), pListenSession);

	return pListenSession->GetHandle();
}

session_handle bnf::CreateListen( const char* host, int port, int waittimeout,
								 rnSocketIOHandler* func, size_t receive_buffer_size, size_t send_buffer_size)
{
	std::string host_str = (host != NULL) ? host : "0.0.0.0";
	return CreateListen(host_str, port, waittimeout, func, receive_buffer_size, send_buffer_size);
}

void bnf::RemoveSession( session_handle handle )
{
	remove_session_queue_.push_signal(handle);
}

void bnf::CloseListen( session_handle handle )
{
	map_session_list_t::iterator it = tcp_listen_session_list_.find(handle);
	if (it == tcp_listen_session_list_.end())
	{
		return;
	}

	delete it->second;
	tcp_listen_session_list_.erase(it);
}

void bnf::Stop()
{
	stopped_ = true;

	// listen 종료
	{
		boost::recursive_mutex::scoped_lock lock1( tcp_listen_session_list_.getmutex() );
		BOOST_FOREACH(map_session_list_t::value_type p, tcp_listen_session_list_)
		{
			((ListenSession *)p.second)->Close();
		}
		// boost::recursive_mutex::scoped_lock lock2( domain_listen_session_list_.getmutex() );
		// BOOST_FOREACH(map_session_list_t::value_type p, domain_listen_session_list_)
		// {
		// 	((ListenSession *)p.second)->Close();
		// }
	}


	// {
	// 	boost::recursive_mutex::scoped_lock lock( listen_session_list_.getmutex() );
	// 	BOOST_FOREACH(map_session_list_t::value_type p, listen_session_list_)
	// 	{
	// 		((ListenSession *)p.second)->JoinAcceptThread();
	// 	}
	// }
	// 
	// {
	// 	boost::recursive_mutex::scoped_lock lock( listen_session_list_.getmutex() );
	// 	BOOST_FOREACH(map_session_list_t::value_type p, listen_session_list_)
	// 	{
	// 		delete p.second;
	// 	}
	// 	listen_session_list_.clear();
	// }


	{
		// session
		boost::recursive_mutex::scoped_lock lock1( tcp_session_list_.getmutex() );
		BOOST_FOREACH(map_session_list_t::value_type p, tcp_session_list_)
		{
			((rnSocketIOService *)p.second)->Close( "server stop." );
		}
		// boost::recursive_mutex::scoped_lock lock2( domain_session_list_.getmutex() );
		// BOOST_FOREACH(map_session_list_t::value_type p, domain_session_list_)
		// {
		// 	((rnSocketIOService *)p.second)->Close( "server stop." );
		// }
	}

	{
		// connect session
		boost::recursive_mutex::scoped_lock lock1( tcp_connect_session_list_.getmutex() );
		BOOST_FOREACH(map_session_list_t::value_type p, tcp_connect_session_list_)
		{
			((rnSocketIOService *)p.second)->Close( "server stop." );
		}
		// boost::recursive_mutex::scoped_lock lock2( domain_connect_session_list_.getmutex() );
		// BOOST_FOREACH(map_session_list_t::value_type p, domain_connect_session_list_)
		// {
		// 	((rnSocketIOService *)p.second)->Close( "server stop." );
		// }
	}

	{
		// timer session
		boost::recursive_mutex::scoped_lock lock( timer_session_list_.getmutex() );
		BOOST_FOREACH(map_session_list_t::value_type p, timer_session_list_)
		{
			((TimerSession*)p.second)->Close();
		}
	}

	// io service thread 종료
	io_service_work_.reset();
}

void bnf::Clear()
{
	static bool _exit_flag = false;

	if (_exit_flag)
		return;

	_exit_flag = true;

	tcp_session_list_.clear();
	tcp_connect_session_list_.clear();
	tcp_listen_session_list_.clear();
	// domain_session_list_.clear();
	// domain_connect_session_list_.clear();
	// domain_listen_session_list_.clear();
	timer_session_list_.clear();

	tcp_session_buf_.clear();
	tcp_connect_session_buf_.clear();
	// domain_session_buf_.clear();
	// domain_connect_session_buf_.clear();
}

session_handle bnf::CreateConnect( std::string& host, std::string& port, void* user_data )
{
	if( stopped_ == true )
		return SessionBase::INVALID_SESSION_HANDLE;

	session_handle shandle;
	if( tcp_connect_session_seq_.pop(shandle) == false )
	{
		growConnectSessionBuffer();
	}

	rnSocketIOServiceTcp* pSession = (rnSocketIOServiceTcp*)&tcp_connect_session_buf_[shandle - CONNECT_SESSION_INDENTIFY];
	pSession->Open(shandle);
	boost::asio::ip::tcp::socket& socket = pSession->Socket();

	boost::asio::ip::tcp::resolver resolver(io_service_);
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
		tcp_connect_session_seq_.push(shandle);
		pSession->error_message_ = ec.message();
		return SessionBase::INVALID_SESSION_HANDLE;
	}

	tcp_connect_session_list_.insert(pSession->GetHandle(), pSession);

	pSession->SetUserData(user_data);
	pSession->SetType(SessionBase::CONNECT_SESSION);
	pSession->_getIp();
	pSession->IncRefCount();

	LOG_INFO( "bnf [%s] CreateConnect - connected. fd: %d, handle: %d", pSession->ip().c_str(), pSession->Socket().native(), pSession->GetHandle() );

	pSession->Run();

	return pSession->GetHandle();
}

session_handle bnf::CreateConnect( std::string& host, int port, void* user_data )
{
	std::string port_str = (boost::format("%d") % port).str();
	return CreateConnect(host, port_str, user_data);
}

session_handle bnf::CreateConnect( const char* host, int port, void* user_data )
{
	std::string host_str = host;
	std::string port_str = (boost::format("%d") % port).str();
	return CreateConnect(host_str, port_str, user_data);
}

session_handle bnf::CreateAsyncConnect( std::string& host, int port, void* user_data)
{
	if( stopped_ == true )
		return SessionBase::INVALID_SESSION_HANDLE;

	session_handle shandle;
	if( tcp_connect_session_seq_.pop(shandle) == false )
	{
		growConnectSessionBuffer();
	}

	rnSocketIOService *pSession = &tcp_connect_session_buf_[shandle - CONNECT_SESSION_INDENTIFY];
	pSession->Open(shandle);

	pSession->SetUserData(user_data);
	pSession->SetType(SessionBase::CONNECT_SESSION);
	pSession->IncRefCount();

	tcp_connect_session_list_.insert(pSession->GetHandle(), pSession);

	if( pSession->AsyncConnect( host, port ) == false )
	{
		tcp_connect_session_list_.erase( pSession->GetHandle() );

		tcp_connect_session_seq_.push(shandle);
		return SessionBase::INVALID_SESSION_HANDLE;
	}

	return pSession->GetHandle();
}

SessionBase* bnf::GetSessionPointer(session_handle handle)
{
	map_session_list_t::iterator it;

	switch (handle & SESSION_HANDLE_CHECKSUM)
	{
	case SESSION_INDENTIFY:
		it = tcp_session_list_.find(handle);
		if (it != tcp_session_list_.end())
		{
			return it->second;
		}
		break;

	case CONNECT_SESSION_INDENTIFY:
		it = tcp_connect_session_list_.find(handle);
		if (it != tcp_connect_session_list_.end())
		{
			return it->second;
		}
		break;

	case TIMER_SESSION_INDENTIFY:
		it = timer_session_list_.find(handle);
		if (it != timer_session_list_.end())
		{
			return it->second;
		}
		break;

	case LISTEN_SESSION_INDENTIFY:
		it = tcp_listen_session_list_.find(handle);
		if (it != tcp_listen_session_list_.end())
		{
			return it->second;
		}
		break;
	// case DOMAIN_SESSION_INDENTIFY:
	// 	it = domain_session_list_.find(handle);
	// 	if (it != domain_session_list_.end())
	// 	{
	// 		return it->second;
	// 	}
	// 	break;

	// case DOMAIN_CONNECT_SESSION_INDENTIFY:
	// 	it = domain_connect_session_list_.find(handle);
	// 	if (it != domain_connect_session_list_.end())
	// 	{
	// 		return it->second;
	// 	}
	// 	break;

	// case DOMAIN_LISTEN_SESSION_INDENTIFY:
	// 	it = domain_listen_session_list_.find(handle);
	// 	if (it != domain_listen_session_list_.end())
	// 	{
	// 		return it->second;
	// 	}
	// 	break;
	}

	return NULL;
}

session_handle bnf::CreateTimer( int msec, void *user_data )
{
	if( stopped_ == true )
		return SessionBase::INVALID_SESSION_HANDLE;

	SessionBase* pSession = new TimerSession(io_service_, msec, msec);
	pSession->SetUserData(user_data);

	timer_session_list_.insert(pSession->GetHandle(), pSession);

	return pSession->GetHandle();
}

session_handle bnf::CreateTimerPeriod( int start_msec, int period_msec, void *user_data )
{
	if( stopped_ == true )
		return SessionBase::INVALID_SESSION_HANDLE;

	SessionBase* pSession = new TimerSession(io_service_, start_msec, period_msec);
	pSession->SetUserData(user_data);

	timer_session_list_.insert(pSession->GetHandle(), pSession);

	return pSession->GetHandle();
}

void bnf::growSessionBuffer()
{
	for (tINT i = 0; i < SEQ_MANAGER_GROW_INDEX_COUNT; ++i)
	{
		rnSocketIOServiceTcp* p = new rnSocketIOServiceTcp(io_service_);
		tcp_session_buf_.push_back(p);
	}
}

void bnf::growConnectSessionBuffer()
{
	for (tINT i = 0; i < SEQ_MANAGER_GROW_INDEX_COUNT; ++i)
	{
		rnSocketIOServiceTcp* p = new rnSocketIOServiceTcp(io_service_);
		tcp_connect_session_buf_.push_back(p);
	}
}


// session_handle bnf::CreateListenDomain( int port, int waittimeout, rnSocketIOHandler* func, size_t receive_buffer_size, size_t send_buffer_size)
// {
// 	if( stopped_ == true )
// 		return SessionBase::INVALID_SESSION_HANDLE;
// 
// 	ListenSessionDomain *pListenSession = new ListenSessionDomain(io_service_, waittimeout);
// 
// 	std::string host(UNIX_DOMAIN_SOCKET_IP);
// 	if( false == pListenSession->Run(host, port, func, receive_buffer_size, send_buffer_size) )
// 	{
// 		delete pListenSession;
// 		return SessionBase::INVALID_SESSION_HANDLE;
// 	}
// 
// 	domain_listen_session_list_.insert(pListenSession->GetHandle(), pListenSession);
// 
// 	return pListenSession->GetHandle();
// }


// void bnf::CloseListenDomain( session_handle handle )
// {
// 	map_session_list_t::iterator it = domain_listen_session_list_.find(handle);
// 	if (it == domain_listen_session_list_.end())
// 	{
// 		return;
// 	}
// 
// 	delete it->second;
// 	domain_listen_session_list_.erase(it);
// }

// session_handle bnf::CreateConnectDomain( int port, void* user_data )
// {
// 	if( stopped_ == true )
// 		return SessionBase::INVALID_SESSION_HANDLE;
// 
// 	session_handle shandle;
// 	if( domain_connect_session_seq_.pop(shandle) == false )
// 	{
// 		growConnectSessionBufferDomain();
// 	}
// 	rnSocketIOServiceDomain* pSession = (rnSocketIOServiceDomain*)&domain_connect_session_buf_[shandle - DOMAIN_CONNECT_SESSION_INDENTIFY];
// 	pSession->Open(shandle);
// 	boost::asio::local::stream_protocol::socket& socket = pSession->Socket();
// 
// 	boost::system::error_code ec = boost::asio::error::host_not_found;
// 	socket.close();
// 
// 	std::ostringstream oss;
// 	oss << UNIX_DOMAIN_SOCKET_FILE << port;
// 	std::string port_string = oss.str();
// 
// 	socket.connect(port_string, ec);
// 	if (ec)
// 	{
// 		socket.close();
// 		domain_connect_session_seq_.push(shandle);
// 		pSession->error_message_ = ec.message();
// 		return SessionBase::INVALID_SESSION_HANDLE;
// 	}
// 
// 	domain_connect_session_list_.insert(pSession->GetHandle(), pSession);
// 
// 	pSession->SetUserData(user_data);
// 	pSession->SetType(SessionBase::CONNECT_SESSION);
// 	pSession->_getIp();
// 	pSession->IncRefCount();
// 
// 	LOG_INFO( "bnf [%s] CreateConnectDomain - connected. fd: %d, handle: %d", pSession->ip().c_str(), pSession->Socket().native(), pSession->GetHandle() );
// 
// 	pSession->Run();
// 
// 	return pSession->GetHandle();
// }

// void bnf::growSessionBufferDomain()
// {
// 	for (tINT i = 0; i < SEQ_MANAGER_GROW_INDEX_COUNT; ++i)
// 	{
// 		rnSocketIOServiceDomain* p = new rnSocketIOServiceDomain(io_service_);
// 		domain_session_buf_.push_back(p);
// 	}
// }

// void bnf::growConnectSessionBufferDomain()
// {
// 	for (tINT i = 0; i < SEQ_MANAGER_GROW_INDEX_COUNT; ++i)
// 	{
// 		rnSocketIOServiceDomain* p = new rnSocketIOServiceDomain(io_service_);
// 		domain_connect_session_buf_.push_back(p);
// 	}
// }
