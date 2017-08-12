#include "stdafx.h"

#include <boost/format.hpp>

#include "Config_INI.h"

#include "ccenter.h"
#include "ccenter_accept.h"
#include "localannounce.h"
#include "log.h"
#include "logger.h"
#include "gmapsetting.h"

#include "ProcessCheck.h"

#include "myhaSlave.h"

RnadomNumberGenerator sRandom;

tINT					myhaSlave::groupid_ = 0;
session_handle			myhaSlave::current_connect_count_handle_ = SessionBase::INVALID_SESSION_HANDLE;
session_handle			myhaSlave::connect_login_handle_ = SessionBase::INVALID_SESSION_HANDLE;
session_handle			myhaSlave::connect_monitor_handle_ = SessionBase::INVALID_SESSION_HANDLE;
session_handle			myhaSlave::service_expire_time_handle_ = SessionBase::INVALID_SESSION_HANDLE;
session_handle			myhaSlave::connect_billing_handle_ = SessionBase::INVALID_SESSION_HANDLE;
session_handle			myhaSlave::update_timer_ = SessionBase::INVALID_SESSION_HANDLE;
session_handle			myhaSlave::chage_day_handle_ = SessionBase::INVALID_SESSION_HANDLE;
session_handle			myhaSlave::stop_roulette_handle_ = SessionBase::INVALID_SESSION_HANDLE;
session_handle			myhaSlave::event_seso_time_handle_ = SessionBase::INVALID_SESSION_HANDLE;

rnIpData				myhaSlave::ip_data_;

tINT					myhaSlave::users_count_max_ = 0;
tINT					myhaSlave::crs_users_count_max_ = 0;
tINT					myhaSlave::nhn_users_count_max_ = 0;

// CommonConfig			Center::common_config_;
CServerSessionManager	myhaSlave::server_session_manager_;
// CLogin					Center::login_;
CCenterAccept			myhaSlave::center_accept_;
CMonitor				myhaSlave::monitor_;
CBilling				myhaSlave::billing_;
TimerClass2				myhaSlave::timer_class_;
InstanceMapManager		myhaSlave::instance_map_manager_;
// MobServerManager		Center::mob_server_manager_;
tTIME					myhaSlave::global_time_;
tINT					myhaSlave::connect_flag_;
InstanceCoolTimeManager myhaSlave::instance_cooltime_manager_;

// 임시
tINT					temp_day;

tBOOL					myhaSlave::event_flag_for_sesotime_ = cFALSE;
tINT					myhaSlave::event_flag_for_seso_persent_ = EVENT_FLAG_SESO_TIME;
tINT					myhaSlave::event_flag_for_sesotime_start_clock_ = 20;
tINT					myhaSlave::event_flag_for_sesotime_end_clock_ = 23;

TDatabaseInfo			myhaSlave::gamedb_info_;
// CDBWorker				Center::dbWorker_;
tBOOL					myhaSlave::server_init_ = cFALSE;

void TimerClass2::operate(rnSocketIOService* service)
{
	myhaSlave::processTimerSession((TimerSession*)service);
}


tBOOL myhaSlave::init(tINT groupid)
{
	global_time_ = time(NULL);

	groupid_ = groupid;

	initSignal();

	return cTRUE;
}

tBOOL myhaSlave::initService()
{
	sRandom.seed(time(NULL));

	session_handle shandle;

	shandle = bnf::instance()->CreateConnect(Config::INI::Instance()->getMasterIP().c_str(), MONITOR_FOR_CENTER, &monitor_);
	if (shandle == SessionBase::INVALID_SESSION_HANDLE)
	{
		LOG_ERROR("Can't connect monitor server. Host=[ '%s:%d' ]", Config::INI::Instance()->getMasterIP().c_str(), MONITOR_FOR_CENTER);
		return cFALSE;
	}
	rnSocketIOService *mService = (rnSocketIOService*)bnf::instance()->GetSessionPointer(shandle);
	monitor_.init(mService);

	update_timer_ = bnf::instance()->CreateTimer(1000, &timer_class_);

	return cTRUE;
}

tBOOL myhaSlave::run()
{
	bnf::instance()->Init();

	LOG_DEBUG("start service center(%d)", groupid_);
	printf(">>>> start service center(%d)\n", groupid_);

	if (initService() == cFALSE)
		return cFALSE;

	LOG_DEBUG("DO2 CENTER SERVER ..... OK (with boost_%s)", BOOST_LIB_VERSION);
	printf("\t\t>>>> DO2 CENTER SERVER ..... OK (with boost_%s)\n", BOOST_LIB_VERSION);

	bnf::instance()->Run();

	bnf::instance()->Clear();

	LOG_DEBUG("end service center(%d)", groupid_);
	printf(">>>> end service center(%d)\n", groupid_);

	return cTRUE;
}

tBOOL myhaSlave::loadScripts()
{
	return cTRUE;
}

void myhaSlave::Stop()
{
	endService();

	bnf::instance()->Stop();
}

void myhaSlave::setConnectFlag(tINT flag)
{
	connect_flag_ |= flag;

	if (connect_flag_ == CONNECTED_SERVER_ALL)
		connect_flag_ = INIT_SERVER_COMPLETED;

}

tBOOL myhaSlave::endService()
{
	server_session_manager_.deliver(SERVER_TYPE_DODB, CLocalRequest::end());
	while (server_session_manager_.getSessionCount(SERVER_TYPE_DODB) > 0)
		usleep(100000);
	LOG_INFO("center : close all dodb server");

	server_session_manager_.deliver(SERVER_TYPE_GMOB, CLocalRequest::end());
	while (server_session_manager_.getSessionCount(SERVER_TYPE_GMOB) > 0)
		usleep(100000);
	LOG_INFO("center : close all gmob server");

	server_session_manager_.deliver(SERVER_TYPE_GCONTAINER, CLocalRequest::end());
	while (server_session_manager_.getSessionCount(SERVER_TYPE_GCONTAINER) > 0)
		usleep(100000);
	LOG_INFO("center : close all gcontainer server");

	server_session_manager_.deliver(SERVER_TYPE_PROXY, CLocalRequest::end());
	while (server_session_manager_.getSessionCount(SERVER_TYPE_PROXY) > 0)
		usleep(100000);
	LOG_INFO("center : close all proxy server");

	server_session_manager_.deliver(SERVER_TYPE_CHAT, CLocalRequest::end());
	while (server_session_manager_.getSessionCount(SERVER_TYPE_CHAT) > 0)
		usleep(100000);
	LOG_INFO("center : close all Chat server");

	server_session_manager_.deliver(SERVER_TYPE_HCASH, CLocalRequest::end());
	while (server_session_manager_.getSessionCount(SERVER_TYPE_HCASH) > 0)
		usleep(100000);
	LOG_INFO("center : close all hcash server");

	server_session_manager_.deliver(SERVER_TYPE_TCOIN, CLocalRequest::end());
	while (server_session_manager_.getSessionCount(SERVER_TYPE_TCOIN) > 0)
		usleep(100000);
	LOG_INFO("center : close all tcoin server");

	return cTRUE;
}

void myhaSlave::processTimerSession(TimerSession* pSession)
{
	if (pSession->GetHandle() == update_timer_)
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

			monitor_.deliver(MonitorAnnounce::ProcessStatus(2, status));
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

void myhaSlave::setServerInit()
{
	LOG_DEBUG("Center::setServerInit()");
	server_init_ = cTRUE;
}
