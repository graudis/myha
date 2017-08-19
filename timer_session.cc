#include "bnf.h"

#include "timer_session.h"

TimerSession::TimerSession(boost::asio::io_service& io_service, int start_msec, int period_msec)
	: SessionBase(io_service, SessionBase::TIMER_SESSION),
	__timer(io_service)
{
	__millisecond = period_msec;
	__running = true;

	bnf::instance()->__timer_session_seq.pop(_handle);
	__timer.expires_from_now(boost::posix_time::milliseconds(start_msec));
	__timer.async_wait(boost::bind(&TimerSession::handle_process, this, _1));
}

TimerSession::~TimerSession()
{
	bnf::instance()->__timer_session_seq.push(_handle);
}

void TimerSession::handle_process(const boost::system::error_code& error)
{
	if (error || __running == false)
	{
		delete this;
		return;
	}

	bnf::instance()->PutSessionEvent(SessionEvent::ON_TIMER, this);
	__timer.expires_from_now(boost::posix_time::milliseconds(__millisecond));
	__timer.async_wait(boost::bind(&TimerSession::handle_process, this, _1));
}

void TimerSession::Close()
{
	_io_service.post(boost::bind(&TimerSession::Stop, this));
}

void TimerSession::Stop()
{
	__timer.cancel();
	__running = false;
}
