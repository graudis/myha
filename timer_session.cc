#include "bnf.h"

#include "timer_session.h"

TimerSession::TimerSession(boost::asio::io_service& io_service, int start_msec, int period_msec)
	: SessionBase(io_service, SessionBase::TIMER_SESSION),
		timer_(io_service)
{
	millisecond_ = period_msec;
	running_ = true;

	bnf::instance()->timer_session_seq_.pop(handle_);
	timer_.expires_from_now( boost::posix_time::milliseconds( start_msec ) );
	timer_.async_wait( boost::bind( &TimerSession::handle_process, this, _1 ) );
}

TimerSession::~TimerSession()
{
	bnf::instance()->timer_session_seq_.push(handle_);
}


void TimerSession::handle_process(const boost::system::error_code& error)
{
	if (error || running_ == false) {
		delete this;
		return;
	}

	bnf::instance()->PutSessionEvent( SessionEvent::ON_TIMER, this );
	timer_.expires_from_now( boost::posix_time::milliseconds( millisecond_ ) );
	timer_.async_wait( boost::bind( &TimerSession::handle_process, this, _1 ) );
}

void TimerSession::Close()
{
	io_service_.post(boost::bind(&TimerSession::Stop, this));
}

void TimerSession::Stop()
{
	timer_.cancel();
	running_ = false;
}
