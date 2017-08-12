#pragma once

#include "rnsocketioserviceImpl.h"
#include "session_base.h"
//#include "MemoryPoolBase.h"
#include "queue_ts.h"

#include "rnpacket.h"

class ListenSession;
class ListenSessionTcp;
class bnf;


class rnSocketIOService : public SessionBase, public rnSocketIOServiceImpl
{
public:
	friend class ListenSession;
	friend class ListenSessionTcp;
	friend class bnf;

//	typedef boost::shared_ptr<rnSocketIOService>	SP;

public:
	rnSocketIOService(boost::asio::io_service& io_service, session_type type) : SessionBase(io_service, type) {};
	virtual ~rnSocketIOService()	{};

	virtual void							Open(session_handle handle) = 0;
	virtual void							Open(session_handle handle, session_handle listen_handle, int waittimeout) = 0;

	virtual bool							AsyncConnect( const std::string& host, int port ) = 0;

	virtual session_handle					GetListenSessionHandle() = 0;

//	virtual boost::asio::ip::tcp::socket&	Socket() = 0;

	virtual void							Run() = 0;
	virtual void							Close(std::string err_str) = 0;
	virtual void							CloseShutdownBoth( std::string err_str ) = 0;

	virtual void							ShutdownRead() = 0;
	virtual void							ShutdownWrite() = 0;
	virtual void							ShutdownBoth() = 0;

	virtual std::string&					ip() = 0;
	virtual unsigned int					ipnumber() = 0;

	virtual void							deliver(rnPacket::SP packet) = 0;
	virtual void							deliver(rnPacket *pPacket) = 0;

	virtual rnPacket*						GetMessage() = 0;

	virtual bool							isValid() = 0;

	// socket option
	virtual void							ReceiveBufferSize(size_t size) = 0;
	virtual int								ReceiveBufferSize() = 0;
	virtual void							SendBufferSize(size_t size) = 0;
	virtual int								SendBufferSize() = 0;
	virtual void							NoDelayOn() = 0;
	virtual void							NoDelayOff() = 0;
	virtual void							DoNotRouteOn() = 0;
	virtual void							DoNotRouteOff() = 0;
	virtual void							Linger(bool onOff, int linger) = 0;
	virtual void							DebugOn() = 0;
	virtual void							DebugOff() = 0;
	virtual void							KeepAliveOn() = 0;
	virtual void							KeepAliveOff() = 0;
	virtual void							BroadcastOn() = 0;
	virtual void							BroadcastOff() = 0;
	virtual void							NonBlockingIoOn() = 0;
	virtual void							NonBlockingIoOff() = 0;

	virtual void							IncRefCount() = 0;
	virtual void							DecRefCount() = 0;

	virtual tINT							getWirteQueueCount() = 0;

	virtual void							closeOnExecOn() = 0;
	virtual void							closeOnExecOff() = 0;

	virtual bool							isTimeout() = 0;

	virtual int								getReadErrorCode() = 0;

protected:
	virtual void							_getIp() = 0;
};
