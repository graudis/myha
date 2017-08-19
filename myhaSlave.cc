#include "stdafx.h"

#include <boost/format.hpp>

#include "Config_INI.h"

#include "ccenter.h"
#include "ccenter_accept.h"

#include "ProcessCheck.h"

#include "myhaSlave.h"

int32_t					myhaSlave::__group_id = 0;
session_handle			myhaSlave::connect_monitor_handle = SessionBase::INVALID_SESSION_HANDLE;
session_handle			myhaSlave::update_timer = SessionBase::INVALID_SESSION_HANDLE;


CServerSessionManager	myhaSlave::__server_session_manager;
CCenterAccept			myhaSlave::__center_accept;
CMonitor				myhaSlave::__monitor;
TimerClass2				myhaSlave::timer_class;
time_t					myhaSlave::__global_time;
int32_t					myhaSlave::__connect_flag;
bool					myhaSlave::__server_init = false;

void TimerClass2::operate(rnSocketIOService* service)
{
	myhaSlave::processTimerSession((TimerSession*)service);
}

bool myhaSlave::init(int32_t groupid)
{
	__global_time = time(NULL);

	__group_id = groupid;

	initSignal();

	return true;
}

bool myhaSlave::initService()
{
	session_handle shandle;

	shandle = bnf::instance()->CreateConnect(Config::INI::Instance()->getMasterIP().c_str(), MONITOR_FOR_CENTER, &__monitor);
	if (shandle == SessionBase::INVALID_SESSION_HANDLE)
	{
		LOG_ERROR("Can't connect monitor server. Host=[ '%s:%d' ]", Config::INI::Instance()->getMasterIP().c_str(), MONITOR_FOR_CENTER);
		return false;
	}
	rnSocketIOService *mService = (rnSocketIOService*)bnf::instance()->GetSessionPointer(shandle);
	__monitor.init(mService);

	update_timer = bnf::instance()->CreateTimer(1000, &timer_class);

	return true;
}

bool myhaSlave::run()
{
	bnf::instance()->Init();

	LOG_DEBUG("start service center(%d)", __group_id);
	printf(">>>> start service center(%d)\n", __group_id);

	if (initService() == false)
		return false;

	LOG_DEBUG("DO2 CENTER SERVER ..... OK (with boost_%s)", BOOST_LIB_VERSION);
	printf("\t\t>>>> DO2 CENTER SERVER ..... OK (with boost_%s)\n", BOOST_LIB_VERSION);

	bnf::instance()->Run();

	bnf::instance()->Clear();

	LOG_DEBUG("end service center(%d)", __group_id);
	printf(">>>> end service center(%d)\n", __group_id);

	return true;
}

bool myhaSlave::loadScripts()
{
	return true;
}

void myhaSlave::Stop()
{
	endService();

	bnf::instance()->Stop();
}

void myhaSlave::setConnectFlag(int32_t flag)
{
	__connect_flag |= flag;

	if (__connect_flag == CONNECTED_SERVER_ALL)
		__connect_flag = INIT_SERVER_COMPLETED;

}

bool myhaSlave::endService()
{
	return true;
}

void myhaSlave::processTimerSession(TimerSession* pSession)
{
	if (pSession->GetHandle() == update_timer)
	{
		uint16_t port = 3306;

		bool tcp_check = TCPPortCheck(Config::INI::Instance()->getSlaveIP(), port);
		bool proc_check = ProcessCheck();

		// 		if (&monitor_ != NULL)
		{
			bool status = false;
			if (tcp_check == true && proc_check == true)
				status = true;

			LOG_TRACE("Slave is %s.", (status == true) ? "OK" : "NOT OK");

			__monitor.deliver(MonitorAnnounce::ProcessStatus(2, status));
		}
	}
}

void myhaSlave::initSignal()
{
	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	sigaddset(&sigset, SIGTERM);
	sigaddset(&sigset, SIGHUP);

	pthread_sigmask(SIG_BLOCK, &sigset, NULL);

	boost::thread signal_handler(boost::bind(myhaSlave::signalHandler));
}

void myhaSlave::signalHandler()
{
	while (1)
	{
		sigset_t sigset;
		sigemptyset(&sigset);

		sigaddset(&sigset, SIGINT);
		sigaddset(&sigset, SIGTERM);
		sigaddset(&sigset, SIGHUP);

		int sig;
		int result = sigwait(&sigset, &sig);
		if (result != 0)
		{
			LOG_ERROR("sigwait failed. error: %d", result);
			continue;
		}

		LOG_INFO("receive signal with %d", sig);

		switch (sig) {
		case SIGINT:
		case SIGTERM:
		case SIGHUP:
			myhaSlave::Stop();
			break;
		};
	}
}
