#pragma once

#include "rnsocketioservice.h"
#include "session_base.h"
#include "MemoryPoolBase.h"
#include "queue_ts.h"

#include "rnpacket.h"

class ListenSessionTcp;
class bnf;


class rnSocketIOServiceTcp : public rnSocketIOService, public MemoryPoolBase<rnSocketIOServiceTcp>
{
public:
	friend class ListenSessionTcp;
	friend class bnf;

//	typedef boost::shared_ptr<rnSocketIOServiceTcp>	SP;

public:
	rnSocketIOServiceTcp(boost::asio::io_service& io_service);
	virtual ~rnSocketIOServiceTcp();

	virtual void							Open(session_handle handle);
	virtual void							Open(session_handle handle, session_handle listen_handle, int waittimeout);

	virtual bool							AsyncConnect( const std::string& host, int port );

	virtual session_handle					GetListenSessionHandle()	{ return listen_session_handle_; }

	virtual void							Run();
	virtual void							Close(std::string err_str);
	virtual void							CloseShutdownBoth( std::string err_str );

	virtual void							ShutdownRead();
	virtual void							ShutdownWrite();
	virtual void							ShutdownBoth();

	virtual std::string&					ip();
	virtual unsigned int					ipnumber();

	virtual void							deliver(rnPacket::SP packet);
	virtual void							deliver(rnPacket *pPacket)	{ deliver(rnPacket::SP(pPacket)); }

	virtual rnPacket*						GetMessage()				{ return read_queue_.front_pop(); }

	virtual bool							isValid()					{ return (handle_ != INVALID_SESSION_HANDLE && socket_.is_open()); }

	// socket option
	virtual void							ReceiveBufferSize(size_t size);
	virtual int								ReceiveBufferSize();
	virtual void							SendBufferSize(size_t size);
	virtual int								SendBufferSize();
	virtual void							NoDelayOn();
	virtual void							NoDelayOff();
	virtual void							DoNotRouteOn();
	virtual void							DoNotRouteOff();
	virtual void							Linger(bool onOff, int linger);
	virtual void							DebugOn();
	virtual void							DebugOff();
	virtual void							KeepAliveOn();
	virtual void							KeepAliveOff();
	virtual void							BroadcastOn();
	virtual void							BroadcastOff();
	virtual void							NonBlockingIoOn();
	virtual void							NonBlockingIoOff();

	virtual void							IncRefCount();
	virtual void							DecRefCount();

	virtual tINT							getWirteQueueCount() { return write_queue_.size(); }

	virtual void							closeOnExecOn();
	virtual void							closeOnExecOff();

	virtual bool							isTimeout() { return timeout_; }

	virtual int								getReadErrorCode() { return read_error_code_.value(); }


protected:
	virtual void							_getIp();

	boost::asio::ip::tcp::socket&			Socket()					{ return socket_; }

	void							Init();
	void							SetWaitTimer();

	void							HandleAsyncConnect( const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator );
	void							HandleReadHeader(const boost::system::error_code& error);
	void							HandleReadBody(const boost::system::error_code& error);
	void							HandleWrite(const boost::system::error_code& error, std::size_t bytes_transferred);
	void							WaitTimerClose(const boost::system::error_code& error);
	void							__close();
	void							__close_shutdown_both();

private:
	boost::asio::ip::tcp::socket	socket_;
	boost::asio::deadline_timer		wait_timer_;
	int								wait_time_sec_;
	std::string						ip_;
	unsigned int					ipnumber_;

	volatile bool					close_flag_;
	session_handle					listen_session_handle_;

	rnPacket*						now_packet_;
	queue_ts<rnPacket*>				read_queue_;
	std::deque<rnPacket::SP>		write_queue_;
	boost::recursive_mutex			write_queue_mutex_;

	boost::recursive_mutex			close_mutex_;

	boost::recursive_mutex			ref_count_mutex_;
	int								ref_count_;

	bool							timeout_;
	boost::system::error_code		read_error_code_;
};
