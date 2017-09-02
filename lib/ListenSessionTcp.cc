#include "BNF.h"

#include "LogSystem.h"

#include "ListenSessionTcp.h"

ListenSessionTcp::ListenSessionTcp(boost::asio::io_service& io_service, int waittimeout) :
	ListenSession(io_service, SessionBase::LISTEN_SESSION),
	__acceptor(io_service),
	__session_wait_time(waittimeout),
	__ref_count(0)
{
	BNF::instance()->__tcp_listen_session_seq.pop(_handle);
}

ListenSessionTcp::~ListenSessionTcp()
{
	BNF::instance()->__tcp_listen_session_seq.push(_handle);
}

bool ListenSessionTcp::Run(std::string& host, const int port, SocketIOHandler* func, size_t receive_buffer_size, size_t send_buffer_size)
{
	std::ostringstream oss;
	oss << port;

	std::string port_string = oss.str();

	boost::asio::ip::tcp::resolver resolver(_io_service);

	boost::asio::ip::tcp::resolver::query query(host, port_string);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

	boost::system::error_code error;

	__acceptor.open(endpoint.protocol(), error);
	if (error)
	{
		LOG_ERROR("bnf - acceptor_.open error. %s:%d, error: %s", host.c_str(), port, error.message().c_str());
		return false;
	}

	__acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), error);
	if (error)
	{
		LOG_ERROR("bnf - acceptor_.set_option error. %s:%d, error: %s", host.c_str(), port, error.message().c_str());
		return false;
	}

	__acceptor.bind(endpoint, error);
	if (error)
	{
		LOG_ERROR("bnf - acceptor_.bind error. %s:%d, error: %s", host.c_str(), port, error.message().c_str());
		return false;
	}

	__acceptor.listen(boost::asio::socket_base::max_connections, error);
	if (error)
	{
		LOG_ERROR("bnf - acceptor_.listen error. %s:%d, error: %s", host.c_str(), port, error.message().c_str());
		return false;
	}

	closeOnExecOn();

	__function = func;

	session_handle shandle;
	if (BNF::instance()->__tcp_session_seq.pop(shandle) == false)
	{
		BNF::instance()->growSessionBuffer();
		LOG_INFO("session buffer growing.");
	}

	SocketIOServiceTcp* pSession = (SocketIOServiceTcp*)&BNF::instance()->__tcp_session_buf[shandle];
	pSession->Open(shandle, _handle, __session_wait_time);

	IncRefCount();
	__acceptor.async_accept(pSession->Socket(),
		boost::bind(&ListenSessionTcp::HandleAccept, this, pSession, boost::asio::placeholders::error));

	LOG_INFO("bnf - open listen socket. fd: %d, handle: %d", __acceptor.native(), GetHandle());

	return true;
}

void ListenSessionTcp::Close()
{
	_io_service.post(boost::bind(&ListenSessionTcp::HandleClose, this));
}

void ListenSessionTcp::HandleAccept(SocketIOService* pSessionService, const boost::system::error_code& error)
{
	if (!error)
	{
		SocketIOServiceTcp* pSession = (SocketIOServiceTcp*)pSessionService;

		pSession->_getIp();

		// event
		__function->operate(pSession);

		BNF::instance()->__tcp_session_list.insert(pSession->GetHandle(), pSession);
		pSession->Run();

		LOG_INFO("bnf [%s] - accepted. fd: %d, handle: %d", pSession->ip().c_str(), pSession->Socket().native(), pSession->GetHandle());

		session_handle shandle;

		while (1)
		{
			if (BNF::instance()->__tcp_session_seq.pop(shandle) == false)
			{
				BNF::instance()->growSessionBuffer();
				LOG_INFO("session buffer growing.");
			}

			pSession = (SocketIOServiceTcp*)&BNF::instance()->__tcp_session_buf[shandle];
			if (((SocketIOServiceTcp*)pSession)->Socket().is_open() == false)
			{
				break;
			}

			LOG_INFO("bnf - skip SocketIOService. handle: %d, fd: %d", pSession->GetHandle(), pSession->Socket().native());
		}


		pSession->Open(shandle, _handle, __session_wait_time);
		__acceptor.async_accept(pSession->Socket(),
			boost::bind(&ListenSessionTcp::HandleAccept, this, pSession, boost::asio::placeholders::error));
	}
	else
	{
		LOG_ERROR("bnf - HandleAccept error. fd: %d, handle: %d, error: %s", __acceptor.native(), GetHandle(), error.message().c_str());
		DecRefCount();
	}
}

void ListenSessionTcp::HandleClose()
{
	boost::system::error_code error;
	__acceptor.close(error);
}

void ListenSessionTcp::closeOnExecOn()
{
	int oldflags = fcntl(__acceptor.native(), F_GETFD, 0);
	if (oldflags >= 0)
	{
		fcntl(__acceptor.native(), F_SETFD, oldflags | FD_CLOEXEC);
	}
}

void ListenSessionTcp::closeOnExecOff()
{
	int oldflags = fcntl(__acceptor.native(), F_GETFD, 0);
	if (oldflags >= 0)
	{
		fcntl(__acceptor.native(), F_SETFD, oldflags & ~FD_CLOEXEC);
	}
}

void ListenSessionTcp::IncRefCount()
{
	boost::recursive_mutex::scoped_lock lock(__ref_count_mutex);
	__ref_count++;
}

void ListenSessionTcp::DecRefCount()
{
	bool delete_flag = false;

	{
		boost::recursive_mutex::scoped_lock lock(__ref_count_mutex);
		__ref_count--;

		if (0 == __ref_count)
		{
			LOG_INFO("bnf - listen closed. fd: %d, handle: %d", __acceptor.native(), GetHandle());

			delete_flag = true;

		}
	}

	if (delete_flag == true)
		BNF::instance()->RemoveSession(GetHandle());
}
