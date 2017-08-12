#include "bnf.h"

#include "logsystem.h"

#include "rnsocketioserviceTcp.h"

rnSocketIOServiceTcp::rnSocketIOServiceTcp(boost::asio::io_service& io_service)
: rnSocketIOService(io_service, SessionBase::SESSION),
socket_(io_service),
wait_timer_(io_service),
ref_count_( 0 ), timeout_( false )
{
	now_packet_ = new rnPacket;
	close_flag_ = false;
}

rnSocketIOServiceTcp::~rnSocketIOServiceTcp()
{
	if (close_flag_ == false && socket_.is_open())
	{
		boost::system::error_code error;
		socket_.close( error );
	}

	delete now_packet_;
}

void rnSocketIOServiceTcp::Open(session_handle handle)
{
	Init();

	SetHandle(handle);
	listen_session_handle_ = SessionBase::INVALID_SESSION_HANDLE;
}

void rnSocketIOServiceTcp::Open(session_handle handle, session_handle listen_handle, int waittimeout)
{
	Init();

	SetHandle(handle);
	listen_session_handle_ = listen_handle;
	wait_time_sec_ = waittimeout;
}

bool rnSocketIOServiceTcp::AsyncConnect( const std::string& host, int port )
{
	std::ostringstream oss;
	oss << port;

	std::string port_string = oss.str();

	boost::asio::ip::tcp::resolver resolver(io_service_);
	boost::asio::ip::tcp::resolver::query query(host, port_string);

	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

	LOG_DEBUG( "bnf - async connecting to %s:%d. fd: %d, handle: %d", host.c_str(), port, socket_.native(), GetHandle() );

	IncRefCount();
	socket_.async_connect( endpoint, boost::bind( &rnSocketIOServiceTcp::HandleAsyncConnect, this, boost::asio::placeholders::error, ++endpoint_iterator ) );
	return true;
}

void rnSocketIOServiceTcp::Run()
{
	if (wait_time_sec_ > 0)
	{
		SetWaitTimer();
	}

	NoDelayOn();

	IncRefCount();
	boost::asio::async_read(socket_,
		boost::asio::buffer(now_packet_->header(), PACKET_HEADER_SIZE),
		boost::bind(&rnSocketIOServiceTcp::HandleReadHeader, this, boost::asio::placeholders::error ));
}


void rnSocketIOServiceTcp::Close( std::string err_str )
{
	//	error_message_ = err_str;
	LOG_INFO( "bnf [%s] - manual close. fd: %d, handle: %d, message. %s", ip().c_str(), socket_.native(), GetHandle(), err_str.c_str() );

	io_service_.post( boost::bind( &rnSocketIOServiceTcp::__close, this ) );
}


void rnSocketIOServiceTcp::CloseShutdownBoth( std::string err_str )
{
	//	error_message_ = err_str;
	LOG_INFO( "bnf [%s] - manual close. fd: %d, handle: %d, message. %s", ip().c_str(), socket_.native(), GetHandle(), err_str.c_str() );

	io_service_.post( boost::bind( &rnSocketIOServiceTcp::__close_shutdown_both, this ) );
}

void rnSocketIOServiceTcp::deliver( rnPacket::SP packet)
{
	boost::recursive_mutex::scoped_lock close_lock( close_mutex_ );
	if (close_flag_ == true || socket_.is_open() == false)
		return;

	boost::recursive_mutex::scoped_lock lock(write_queue_mutex_);

	write_queue_.push_back(packet);
	if (write_queue_.size() == 1)
	{
		IncRefCount();
		boost::asio::async_write(socket_,
			boost::asio::buffer(packet->header(), packet->size()),
			boost::bind(&rnSocketIOServiceTcp::HandleWrite, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred) );
	}
}

std::string& rnSocketIOServiceTcp::ip()
{
	return ip_;
}

unsigned int rnSocketIOServiceTcp::ipnumber()
{
	return ipnumber_;
}

void rnSocketIOServiceTcp::ShutdownRead()
{
	boost::system::error_code error;
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, error);
}

void rnSocketIOServiceTcp::ShutdownWrite()
{
	boost::system::error_code error;
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, error);
}

void rnSocketIOServiceTcp::ShutdownBoth()
{
	boost::system::error_code error;
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
}

void rnSocketIOServiceTcp::ReceiveBufferSize( size_t size )
{
	boost::asio::socket_base::receive_buffer_size option(size);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

int rnSocketIOServiceTcp::ReceiveBufferSize()
{
	boost::asio::socket_base::receive_buffer_size option;

	boost::system::error_code error;
	socket_.get_option(option, error);
	return option.value();
}

void rnSocketIOServiceTcp::SendBufferSize( size_t size )
{
	boost::asio::socket_base::send_buffer_size option(size);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

int rnSocketIOServiceTcp::SendBufferSize()
{
	boost::asio::socket_base::send_buffer_size option;

	boost::system::error_code error;
	socket_.get_option(option, error);
	return option.value();
}

void rnSocketIOServiceTcp::NoDelayOn()
{
	boost::asio::ip::tcp::no_delay option(true);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::NoDelayOff()
{
	boost::asio::ip::tcp::no_delay option(false);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::DoNotRouteOn()
{
	boost::asio::socket_base::do_not_route option(true);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::DoNotRouteOff()
{
	boost::asio::socket_base::do_not_route option(false);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::Linger( bool onOff, int linger )
{
	boost::asio::socket_base::linger option(onOff, linger);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::DebugOn()
{
	boost::asio::socket_base::debug option(true);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::DebugOff()
{
	boost::asio::socket_base::debug option(false);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::KeepAliveOn()
{
	boost::asio::socket_base::keep_alive option(true);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::KeepAliveOff()
{
	boost::asio::socket_base::keep_alive option(false);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::BroadcastOn()
{
	boost::asio::socket_base::broadcast option(true);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::BroadcastOff()
{
	boost::asio::socket_base::broadcast option(false);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::NonBlockingIoOn()
{
	boost::asio::socket_base::non_blocking_io command(true);

	boost::system::error_code error;
	socket_.io_control(command, error);
}

void rnSocketIOServiceTcp::NonBlockingIoOff()
{
	boost::asio::socket_base::non_blocking_io command(false);

	boost::system::error_code error;
	socket_.io_control(command, error);
}

void rnSocketIOServiceTcp::IncRefCount()
{
	boost::recursive_mutex::scoped_lock lock(ref_count_mutex_);
	ref_count_++;
}

void rnSocketIOServiceTcp::DecRefCount()
{
	boost::recursive_mutex::scoped_lock lock(ref_count_mutex_);
	ref_count_--;

	if( 0 == ref_count_ )
	{
		LOG_INFO( "bnf [%s] - closed. fd: %d, handle: %d", ip().c_str(), socket_.native(), GetHandle() );

		boost::system::error_code error;
		socket_.close( error );
		if( error )
		{
			LOG_ERROR( "bnf [%s] - socket_.close error. error: %s", ip().c_str(), error.message().c_str() );
			close_error_ = true;
		}

		if (user_data_ != NULL)
		{
			read_queue_.push(NULL);
			bnf::instance()->PutSessionEvent( SessionEvent::ON_CLOSE, this );
		}
		else
		{
			bnf::instance()->RemoveSession( GetHandle() );
		}
	}
}

void rnSocketIOServiceTcp::closeOnExecOn()
{
	int oldflags = fcntl (socket_.native(), F_GETFD, 0);
	if ( oldflags >= 0 )
	{
		fcntl( socket_.native(), F_SETFD, oldflags | FD_CLOEXEC );
	}
}

void rnSocketIOServiceTcp::closeOnExecOff()
{
	int oldflags = fcntl (socket_.native(), F_GETFD, 0);
	if ( oldflags >= 0 )
	{
		fcntl( socket_.native(), F_SETFD, oldflags & ~FD_CLOEXEC );
	}
}

void rnSocketIOServiceTcp::_getIp()
{
	boost::system::error_code error;

	const boost::asio::ip::tcp::endpoint& remode_endpoint = socket_.remote_endpoint(error);
	if(error)
	{
		LOG_ERROR( "bnf - socket_.remote_endpoint error. fd: %d, handle: %d, error: %s", socket_.native(), GetHandle(),
			error.message().c_str() );
		return;
	}

	ip_ =  remode_endpoint.address().to_string(error);
	ipnumber_ =  remode_endpoint.address().to_v4().to_ulong();
}

void rnSocketIOServiceTcp::Init()
{
	Linger( 0, 0 );

	// SessionBase
	type_ = 0;
	handle_ = 0;
	user_data_ = NULL;
	error_message_.clear();

	// rnSocketIOServiceTcp
	wait_time_sec_ = 0;

	ip_.clear();
	ipnumber_ = 0;
	close_flag_ = false;
	listen_session_handle_ = 0;

	now_packet_->reset();

	read_queue_.clear();
	write_queue_.clear();
	ref_count_ = 0;

	timeout_ = false;
	close_error_ = false;
}

void rnSocketIOServiceTcp::SetWaitTimer()
{
	wait_timer_.expires_from_now( boost::posix_time::seconds( wait_time_sec_ ) );
	wait_timer_.async_wait( boost::bind( &rnSocketIOServiceTcp::WaitTimerClose, this, _1 ) );
}


void rnSocketIOServiceTcp::HandleAsyncConnect( const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator )
{
	if( !error )
	{
		// 성공
		_getIp();
		LOG_INFO( "bnf [%s] HandleAsyncConnect - connected. fd: %d, handle: %d", ip().c_str(), socket_.native(), GetHandle() );

		bnf::instance()->PutSessionEvent( SessionEvent::ON_CONNECT, this );

		Run();
		DecRefCount();
	}
	else if( endpoint_iterator != boost::asio::ip::tcp::resolver::iterator() )
	{
		// 다른 ip가 있다면 다른 ip로 접속 시도
		boost::system::error_code error;
		socket_.close( error );

		boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

		socket_.async_connect( endpoint, boost::bind( &rnSocketIOServiceTcp::HandleAsyncConnect, this, boost::asio::placeholders::error, ++endpoint_iterator ) );
	}
	else
	{
		// 실패
		LOG_ERROR( "bnf [%s] - connect failed. fd: %d, handle: %d, error: %s", ip().c_str(), socket_.native(), GetHandle(),
			error.message().c_str() );

		bnf::instance()->PutSessionEvent( SessionEvent::ON_CONNECT_FAIL, this );

		__close_shutdown_both();
		DecRefCount();
	}
}

void rnSocketIOServiceTcp::HandleReadHeader(const boost::system::error_code& error)
{
	if (error)
	{
		//		error_message_ = error.message();
		read_error_code_ = error;

		if( error.value() != boost::asio::error::eof )
		{
			LOG_ERROR( "bnf [%s] - HandleReadHeader error. fd: %d, handle: %d, error: %s", ip().c_str(), socket_.native(), GetHandle(), error.message().c_str() );
			__close_shutdown_both();
		}
		else
		{
			__close();
		}

		DecRefCount();
		return;
	}

	// 클라가 패킷을 잘못 보냈을 경우
	if (now_packet_->size() > MAX_PACKET_MESSAGE)
	{
		std::string message = "packet size is too big";
		LOG_ERROR( "bnf [%s] - HandleReadHeader error. fd: %d, handle: %d, error: %s", ip().c_str(), socket_.native(), GetHandle(), message.c_str() );

		__close();
		DecRefCount();
		return;
	}

	if (wait_time_sec_ > 0)
	{
		SetWaitTimer();
	}

	boost::asio::async_read(socket_,
		boost::asio::buffer(now_packet_->data(), now_packet_->dataSize()),
		boost::bind(&rnSocketIOServiceTcp::HandleReadBody, this, boost::asio::placeholders::error));
	// now_packet_->dumpSimple();
}

void rnSocketIOServiceTcp::HandleReadBody(const boost::system::error_code& error)
{
	if (error)
	{
		//		error_message_ = error.message();

		if( error.value() != boost::asio::error::eof )
		{
			LOG_ERROR( "bnf [%s] - HandleReadHeader error. fd: %d, handle: %d, error: %s", ip().c_str(), socket_.native(), GetHandle(), error.message().c_str() );
			__close_shutdown_both();
		}
		else
		{
			__close();
		}
		DecRefCount();
		return;
	}

	read_queue_.push(now_packet_);
	bnf::instance()->PutSessionEvent( SessionEvent::ON_RECEIVE, this );

	now_packet_ = new rnPacket;

	boost::asio::async_read(socket_,
		boost::asio::buffer(now_packet_->header(), PACKET_HEADER_SIZE),
		boost::bind(&rnSocketIOServiceTcp::HandleReadHeader, this, boost::asio::placeholders::error ));
	// now_packet_->dumpSimple();
}

void rnSocketIOServiceTcp::HandleWrite( const boost::system::error_code& error, std::size_t bytes_transferred )
{
	bool error_flag = false;

	{
		boost::recursive_mutex::scoped_lock lock(write_queue_mutex_);
		write_queue_.pop_front();

		if (!error)
		{
			if (write_queue_.empty())
			{
				DecRefCount();
				return;
			}

			rnPacket* packet = write_queue_.front().get();
			boost::asio::async_write(socket_,
				boost::asio::buffer((void *)packet->header(), packet->size()),
				boost::bind(&rnSocketIOServiceTcp::HandleWrite, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred) );
			// packet->dumpSimple();
		}
		else
		{
			LOG_ERROR( "bnf [%s] - HandleWrite error. fd: %d, handle: %d, error: %s", ip().c_str(), socket_.native(), GetHandle(), error.message().c_str() );
			write_queue_.clear();

			error_flag = true;
		}
	}

	if( error_flag == true )
	{
		__close_shutdown_both();
		DecRefCount();
	}

}

void rnSocketIOServiceTcp::WaitTimerClose(const boost::system::error_code& error)
{
	if (error)
	{
		return;
	}

	timeout_ = true;
	//	error_message_ = "wait time out";
	LOG_ERROR( "bnf [%s] - time out. fd: %d, handle: %d", ip().c_str(), socket_.native(), GetHandle() );

	__close_shutdown_both();
}

void rnSocketIOServiceTcp::__close()
{
	boost::recursive_mutex::scoped_lock lock( close_mutex_ );

	if (close_flag_ == true)
		return;

	close_flag_ = true;

	if (wait_time_sec_ > 0)
	{
		wait_timer_.cancel();
	}

	boost::system::error_code error;
	//	socket_.cancel( error );
	socket_.shutdown( boost::asio::ip::tcp::socket::shutdown_receive, error );

	DecRefCount();
}

void rnSocketIOServiceTcp::__close_shutdown_both()
{
	boost::recursive_mutex::scoped_lock lock( close_mutex_ );

	if (close_flag_ == true || socket_.is_open() == false)
		return;

	close_flag_ = true;

	if (wait_time_sec_ > 0)
	{
		wait_timer_.cancel();
	}

	boost::system::error_code error;
	socket_.cancel( error );
	socket_.shutdown( boost::asio::ip::tcp::socket::shutdown_both, error );

	DecRefCount();
}
