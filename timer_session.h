#pragma once

#include "session_base.h"
#include "MemoryPoolBase.h"

class TimerSession : public SessionBase, public MemoryPoolBase<TimerSession>
{
public:
	TimerSession(boost::asio::io_service& io_service, int start_msec, int period_msec);
	~TimerSession();

public:
	void Close();

private:
	void handle_process(const boost::system::error_code& error);
	void Stop();

private:
	boost::asio::deadline_timer __timer;
	int32_t __millisecond;
	bool __running;
};
