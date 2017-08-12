#pragma once

#include <string>
#include <boost/function.hpp>

#include "MemoryPoolBase.h"
#include "seq_manager.h"
#include "map_ts.h"

#include "listen_session.h"
#include "rnsocketiohandler.h"
#include "rnsocketioserviceTcp.h"


class ListenSessionTcp : public ListenSession, public MemoryPoolBase<ListenSessionTcp>
{
public:
	ListenSessionTcp(boost::asio::io_service& io_service, int waittimeout = 0);
	~ListenSessionTcp();

	virtual bool Run(std::string& host, const int port, rnSocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0);

	virtual void Close();

	virtual void closeOnExecOn();
	virtual void closeOnExecOff();

private:
	void HandleAccept(rnSocketIOService* pSession, const boost::system::error_code& error);
	void HandleClose();

	void IncRefCount();
	void DecRefCount();

private:
	boost::asio::ip::tcp::acceptor acceptor_;

	int session_wait_time_;
	rnSocketIOHandler* function_;

	boost::recursive_mutex ref_count_mutex_;
	int ref_count_;
};
