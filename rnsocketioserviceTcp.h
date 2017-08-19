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

public:
	rnSocketIOServiceTcp(boost::asio::io_service& io_service);
	virtual ~rnSocketIOServiceTcp();

	virtual void Open(session_handle handle);
	virtual void Open(session_handle handle, session_handle listen_handle, int waittimeout);

	virtual bool AsyncConnect(const std::string& host, int port);

	virtual session_handle GetListenSessionHandle() { return __listen_session_handle; }

	virtual void Run();
	virtual void Close(std::string err_str);
	virtual void CloseShutdownBoth(std::string err_str);

	virtual void ShutdownRead();
	virtual void ShutdownWrite();
	virtual void ShutdownBoth();

	virtual std::string& ip();
	virtual uint32_t ipnumber();

	virtual void deliver(rnPacket::SP packet);
	virtual void deliver(rnPacket *pPacket) { deliver(rnPacket::SP(pPacket)); }

	virtual rnPacket* GetMessage() { return __read_queue.front_pop(); }

	virtual bool isValid() { return (_handle != INVALID_SESSION_HANDLE && __socket.is_open()); }

	// socket option
	virtual void ReceiveBufferSize(size_t size);
	virtual int32_t ReceiveBufferSize();
	virtual void SendBufferSize(size_t size);
	virtual int32_t SendBufferSize();
	virtual void NoDelayOn();
	virtual void NoDelayOff();
	virtual void DoNotRouteOn();
	virtual void DoNotRouteOff();
	virtual void Linger(bool onOff, int linger);
	virtual void DebugOn();
	virtual void DebugOff();
	virtual void KeepAliveOn();
	virtual void KeepAliveOff();
	virtual void BroadcastOn();
	virtual void BroadcastOff();
	virtual void NonBlockingIoOn();
	virtual void NonBlockingIoOff();

	virtual void IncRefCount();
	virtual void DecRefCount();

	virtual int32_t getWirteQueueCount() { return __write_queue.size(); }

	virtual void closeOnExecOn();
	virtual void closeOnExecOff();

	virtual bool isTimeout() { return __timeout; }

	virtual int getReadErrorCode() { return __read_error_code.value(); }

protected:
	virtual void _getIp();

	boost::asio::ip::tcp::socket& Socket() { return __socket; }

	void Init();
	void SetWaitTimer();

	void HandleAsyncConnect(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	void HandleReadHeader(const boost::system::error_code& error);
	void HandleReadBody(const boost::system::error_code& error);
	void HandleWrite(const boost::system::error_code& error, std::size_t bytes_transferred);
	void WaitTimerClose(const boost::system::error_code& error);
	void __close();
	void __close_shutdown_both();

private:
	boost::asio::ip::tcp::socket __socket;
	boost::asio::deadline_timer __wait_timer;
	int32_t __wait_time_sec;
	std::string __ip;
	uint32_t __ipnumber;

	volatile bool __close_flag;
	session_handle __listen_session_handle;

	rnPacket* __now_packet;
	queue_ts<rnPacket*> __read_queue;
	std::deque<rnPacket::SP> __write_queue;
	boost::recursive_mutex __write_queue_mutex;

	boost::recursive_mutex __close_mutex;

	boost::recursive_mutex __ref_count_mutex;
	int32_t __ref_count;

	bool __timeout;
	boost::system::error_code __read_error_code;
};
