#include "BNF.h"

#include "LogSystem.h"

#include "SocketIOServiceTcp.h"

SocketIOServiceTcp::SocketIOServiceTcp(boost::asio::io_service& io_service)
	: SocketIOService(io_service, SessionBase::SESSION),
	__socket(io_service),
	__wait_timer(io_service),
	__ref_count(0),
	__timeout(false),
	boost_strand_(io_service)
{
	__now_packet = new Packet;
	__close_flag = false;
}

SocketIOServiceTcp::~SocketIOServiceTcp()
{
	if (__close_flag == false && __socket.is_open())
	{
		boost::system::error_code error;
		__socket.close(error);
	}

	delete __now_packet;
}

void SocketIOServiceTcp::Open(session_handle handle)
{
	Init();

	SetHandle(handle);
	__listen_session_handle = SessionBase::INVALID_SESSION_HANDLE;
}

void SocketIOServiceTcp::Open(session_handle handle, session_handle listen_handle, int waittimeout)
{
	Init();

	SetHandle(handle);
	__listen_session_handle = listen_handle;
	__wait_time_sec = waittimeout;
}

bool SocketIOServiceTcp::AsyncConnect(const std::string& host, int port)
{
	std::ostringstream oss;
	oss << port;

	std::string port_string = oss.str();

	boost::asio::ip::tcp::resolver resolver(_io_service);
	boost::asio::ip::tcp::resolver::query query(host, port_string);

	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

	LOG_DEBUG("bnf - async connecting to %s:%d. fd: %d, handle: %d", host.c_str(), port, __socket.native(), GetHandle());

	IncRefCount();
	__socket.async_connect(
		endpoint,
		boost_strand_.wrap(
			boost::bind(&SocketIOServiceTcp::HandleAsyncConnect, this, boost::asio::placeholders::error, ++endpoint_iterator)
		)
	);
	return true;
}

void SocketIOServiceTcp::Run()
{
	if (__wait_time_sec > 0)
	{
		SetWaitTimer();
	}

	NoDelayOn();

	IncRefCount();
	boost::asio::async_read(
		__socket,
		boost::asio::buffer(__now_packet->getHeader(), PACKET_HEADER_SIZE),
		boost_strand_.wrap(
			boost::bind(&SocketIOServiceTcp::HandleReadHeader, this, boost::asio::placeholders::error)
		)
	);
}


void SocketIOServiceTcp::Close(std::string err_str)
{
	//	error_message_ = err_str;
	LOG_INFO("bnf [%s] - manual close. fd: %d, handle: %d, message. %s", ip().c_str(), __socket.native(), GetHandle(), err_str.c_str());

	boost_strand_.post(boost::bind(&SocketIOServiceTcp::__close, this));
}


void SocketIOServiceTcp::CloseShutdownBoth(std::string err_str)
{
	//	error_message_ = err_str;
	LOG_INFO("bnf [%s] - manual close. fd: %d, handle: %d, message. %s", ip().c_str(), __socket.native(), GetHandle(), err_str.c_str());

	boost_strand_.post(boost::bind(&SocketIOServiceTcp::__close_shutdown_both, this));
}

void SocketIOServiceTcp::deliver(Packet::SP packet)
{
// 	boost::recursive_mutex::scoped_lock close_lock(__close_mutex);
	if (__close_flag == true || __socket.is_open() == false)
		return;

// 	boost::recursive_mutex::scoped_lock lock(__write_queue_mutex); 
	
	__write_queue.push_back(packet);
	if (__write_queue.size() == 1)
	{
		IncRefCount();
		boost::asio::async_write(
			__socket,
			boost::asio::buffer(packet->getHeader(), packet->getSize()),
			boost_strand_.wrap(
				boost::bind(&SocketIOServiceTcp::HandleWrite, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
			)
		);
	}
}

std::string& SocketIOServiceTcp::ip()
{
	return __ip;
}

unsigned int SocketIOServiceTcp::ipnumber()
{
	return __ipnumber;
}

void SocketIOServiceTcp::ShutdownRead()
{
	boost::system::error_code error;
	__socket.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, error);
}

void SocketIOServiceTcp::ShutdownWrite()
{
	boost::system::error_code error;
	__socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, error);
}

void SocketIOServiceTcp::ShutdownBoth()
{
	boost::system::error_code error;
	__socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
}

void SocketIOServiceTcp::ReceiveBufferSize(size_t size)
{
	boost::asio::socket_base::receive_buffer_size option(size);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

int SocketIOServiceTcp::ReceiveBufferSize()
{
	boost::asio::socket_base::receive_buffer_size option;

	boost::system::error_code error;
	__socket.get_option(option, error);
	return option.value();
}

void SocketIOServiceTcp::SendBufferSize(size_t size)
{
	boost::asio::socket_base::send_buffer_size option(size);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

int SocketIOServiceTcp::SendBufferSize()
{
	boost::asio::socket_base::send_buffer_size option;

	boost::system::error_code error;
	__socket.get_option(option, error);
	return option.value();
}

void SocketIOServiceTcp::NoDelayOn()
{
	boost::asio::ip::tcp::no_delay option(true);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

void SocketIOServiceTcp::NoDelayOff()
{
	boost::asio::ip::tcp::no_delay option(false);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

void SocketIOServiceTcp::DoNotRouteOn()
{
	boost::asio::socket_base::do_not_route option(true);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

void SocketIOServiceTcp::DoNotRouteOff()
{
	boost::asio::socket_base::do_not_route option(false);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

void SocketIOServiceTcp::Linger(bool onOff, int linger)
{
	boost::asio::socket_base::linger option(onOff, linger);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

void SocketIOServiceTcp::DebugOn()
{
	boost::asio::socket_base::debug option(true);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

void SocketIOServiceTcp::DebugOff()
{
	boost::asio::socket_base::debug option(false);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

void SocketIOServiceTcp::KeepAliveOn()
{
	boost::asio::socket_base::keep_alive option(true);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

void SocketIOServiceTcp::KeepAliveOff()
{
	boost::asio::socket_base::keep_alive option(false);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

void SocketIOServiceTcp::BroadcastOn()
{
	boost::asio::socket_base::broadcast option(true);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

void SocketIOServiceTcp::BroadcastOff()
{
	boost::asio::socket_base::broadcast option(false);

	boost::system::error_code error;
	__socket.set_option(option, error);
}

void SocketIOServiceTcp::NonBlockingIoOn()
{
	boost::asio::socket_base::non_blocking_io command(true);

	boost::system::error_code error;
	__socket.io_control(command, error);
}

void SocketIOServiceTcp::NonBlockingIoOff()
{
	boost::asio::socket_base::non_blocking_io command(false);

	boost::system::error_code error;
	__socket.io_control(command, error);
}

void SocketIOServiceTcp::IncRefCount()
{
// 	boost::recursive_mutex::scoped_lock lock(__ref_count_mutex);
	__ref_count++;
}

void SocketIOServiceTcp::DecRefCount()
{
// 	boost::recursive_mutex::scoped_lock lock(__ref_count_mutex);
	__ref_count--;

	if (0 == __ref_count)
	{
		LOG_INFO("bnf [%s] - closed. fd: %d, handle: %d", ip().c_str(), __socket.native(), GetHandle());

		boost::system::error_code error;
		__socket.close(error);
		if (error)
		{
			LOG_ERROR("bnf [%s] - socket_.close error. error: %s", ip().c_str(), error.message().c_str());
			_close_error = true;
		}

		if (_user_data != NULL)
		{
			__read_queue.push(NULL);
			BNF::instance()->PutSessionEvent(SessionEvent::ON_CLOSE, this);
		}
		else
		{
			BNF::instance()->RemoveSession(GetHandle());
		}
	}
}

void SocketIOServiceTcp::closeOnExecOn()
{
	int oldflags = fcntl(__socket.native(), F_GETFD, 0);
	if (oldflags >= 0)
	{
		fcntl(__socket.native(), F_SETFD, oldflags | FD_CLOEXEC);
	}
}

void SocketIOServiceTcp::closeOnExecOff()
{
	int oldflags = fcntl(__socket.native(), F_GETFD, 0);
	if (oldflags >= 0)
	{
		fcntl(__socket.native(), F_SETFD, oldflags & ~FD_CLOEXEC);
	}
}

void SocketIOServiceTcp::_getIp()
{
	boost::system::error_code error;

	const boost::asio::ip::tcp::endpoint& remode_endpoint = __socket.remote_endpoint(error);
	if (error)
	{
		LOG_ERROR("bnf - socket_.remote_endpoint error. fd: %d, handle: %d, error: %s", __socket.native(), GetHandle(),
			error.message().c_str());
		return;
	}

	__ip = remode_endpoint.address().to_string(error);
	__ipnumber = remode_endpoint.address().to_v4().to_ulong();
}

void SocketIOServiceTcp::Init()
{
	Linger(0, 0);

	// SessionBase
	_type = 0;
	_handle = 0;
	_user_data = NULL;
	_error_message.clear();

	// rnSocketIOServiceTcp
	__wait_time_sec = 0;

	__ip.clear();
	__ipnumber = 0;
	__close_flag = false;
	__listen_session_handle = 0;

	__now_packet->reset();

	__read_queue.clear();
	__write_queue.clear();
	__ref_count = 0;

	__timeout = false;
	_close_error = false;
}

void SocketIOServiceTcp::SetWaitTimer()
{
	__wait_timer.expires_from_now(boost::posix_time::seconds(__wait_time_sec));
	__wait_timer.async_wait(
		boost_strand_.wrap(
			boost::bind(&SocketIOServiceTcp::WaitTimerClose, this, _1)
		)
	);
}


void SocketIOServiceTcp::HandleAsyncConnect(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
	if (!error)
	{
		// 성공
		_getIp();
		LOG_INFO("bnf [%s] HandleAsyncConnect - connected. fd: %d, handle: %d", ip().c_str(), __socket.native(), GetHandle());

		BNF::instance()->PutSessionEvent(SessionEvent::ON_CONNECT, this);

		Run();
		DecRefCount();
	}
	else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator())
	{
		// 다른 ip가 있다면 다른 ip로 접속 시도
		boost::system::error_code error;
		__socket.close(error);

		boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

		__socket.async_connect(
			endpoint,
			boost_strand_.wrap(
				boost::bind(&SocketIOServiceTcp::HandleAsyncConnect, this, boost::asio::placeholders::error, ++endpoint_iterator)
			)
		);
	}
	else
	{
		// 실패
		LOG_ERROR("bnf [%s] - connect failed. fd: %d, handle: %d, error: %s", ip().c_str(), __socket.native(), GetHandle(),
			error.message().c_str());

		BNF::instance()->PutSessionEvent(SessionEvent::ON_CONNECT_FAIL, this);

		__close_shutdown_both();
		DecRefCount();
	}
}

void SocketIOServiceTcp::HandleReadHeader(const boost::system::error_code& error)
{
	if (error)
	{
		//		error_message_ = error.message();
		__read_error_code = error;

		if (error.value() != boost::asio::error::eof)
		{
			LOG_ERROR("bnf [%s] - HandleReadHeader error. fd: %d, handle: %d, error: %s", ip().c_str(), __socket.native(), GetHandle(), error.message().c_str());
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
	if (__now_packet->getSize() > MAX_PACKET_MESSAGE)
	{
		std::string message = "packet size is too big";
		LOG_ERROR("bnf [%s] - HandleReadHeader error. fd: %d, handle: %d, error: %s", ip().c_str(), __socket.native(), GetHandle(), message.c_str());

		__close();
		DecRefCount();
		return;
	}

	if (__wait_time_sec > 0)
	{
		SetWaitTimer();
	}

	boost::asio::async_read(
		__socket,
		boost::asio::buffer(__now_packet->data(), __now_packet->dataSize()),
		boost_strand_.wrap(
			boost::bind(&SocketIOServiceTcp::HandleReadBody, this, boost::asio::placeholders::error)
		)
	);
	// now_packet_->dumpSimple();
}

void SocketIOServiceTcp::HandleReadBody(const boost::system::error_code& error)
{
	if (error)
	{
		//		error_message_ = error.message();

		if (error.value() != boost::asio::error::eof)
		{
			LOG_ERROR("bnf [%s] - HandleReadHeader error. fd: %d, handle: %d, error: %s", ip().c_str(), __socket.native(), GetHandle(), error.message().c_str());
			__close_shutdown_both();
		}
		else
		{
			__close();
		}
		DecRefCount();
		return;
	}

	__read_queue.push(__now_packet);
	BNF::instance()->PutSessionEvent(SessionEvent::ON_RECEIVE, this);

	__now_packet = new Packet;

	boost::asio::async_read(
		__socket,
		boost::asio::buffer(__now_packet->getHeader(), PACKET_HEADER_SIZE),
		boost_strand_.wrap(
			boost::bind(&SocketIOServiceTcp::HandleReadHeader, this, boost::asio::placeholders::error)
		)
	);
	// now_packet_->dumpSimple();
}

void SocketIOServiceTcp::HandleWrite(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	bool error_flag = false;

	{
// 		boost::recursive_mutex::scoped_lock lock(__write_queue_mutex);
		__write_queue.pop_front();

		if (!error)
		{
			if (__write_queue.empty())
			{
				DecRefCount();
				return;
			}

			Packet* packet = __write_queue.front().get();
			boost::asio::async_write(
				__socket,
				boost::asio::buffer((void *)packet->getHeader(), packet->getSize()),
				boost_strand_.wrap(
					boost::bind(&SocketIOServiceTcp::HandleWrite, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
				)
			);
			// packet->dumpSimple();
		}
		else
		{
			LOG_ERROR("bnf [%s] - HandleWrite error. fd: %d, handle: %d, error: %s", ip().c_str(), __socket.native(), GetHandle(), error.message().c_str());
			__write_queue.clear();

			error_flag = true;
		}
	}

	if (error_flag == true)
	{
		__close_shutdown_both();
		DecRefCount();
	}
}

void SocketIOServiceTcp::WaitTimerClose(const boost::system::error_code& error)
{
	if (error)
	{
		return;
	}

	__timeout = true;
	//	error_message_ = "wait time out";
	LOG_ERROR("bnf [%s] - time out. fd: %d, handle: %d", ip().c_str(), __socket.native(), GetHandle());

	__close_shutdown_both();
}

void SocketIOServiceTcp::__close()
{
// 	boost::recursive_mutex::scoped_lock lock(__close_mutex);

	if (__close_flag == true)
		return;

	__close_flag = true;

	if (__wait_time_sec > 0)
	{
		__wait_timer.cancel();
	}

	boost::system::error_code error;
	//	socket_.cancel( error );
	__socket.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, error);

	DecRefCount();
}

void SocketIOServiceTcp::__close_shutdown_both()
{
// 	boost::recursive_mutex::scoped_lock lock(__close_mutex); 
	
	if (__close_flag == true || __socket.is_open() == false)
		return;

	__close_flag = true;

	if (__wait_time_sec > 0)
	{
		__wait_timer.cancel();
	}

	boost::system::error_code error;
	__socket.cancel(error);
	__socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);

	DecRefCount();
}
