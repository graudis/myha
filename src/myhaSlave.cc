﻿#include "stdafx.h"

#include <boost/format.hpp>

#include "Config_INI.h"

#include "ccenter.h"

#include "ProcessCheck.h"

#include "myhaSlave.h"

int32_t					myhaSlave::__group_id = 0;
session_handle			myhaSlave::connect_monitor_handle = SessionBase::INVALID_SESSION_HANDLE;
session_handle			myhaSlave::update_timer = SessionBase::INVALID_SESSION_HANDLE;


CServerSessionManager	myhaSlave::__server_session_manager;
MasterClient			myhaSlave::__master_client;
TimerClass2				myhaSlave::timer_class;
time_t					myhaSlave::__global_time;
int32_t					myhaSlave::__connect_flag;
bool					myhaSlave::__server_init = false;

void TimerClass2::operate(SocketIOService* service)
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

	shandle = BNF::instance()->CreateConnect(Config::INI::Instance()->getMasterIP().c_str(), MASTER_FOR_SLAVE, &__master_client);
	if (shandle == SessionBase::INVALID_SESSION_HANDLE)
	{
		LOG_ERROR("Can't connect monitor server. Host=[ '%s:%d' ]", Config::INI::Instance()->getMasterIP().c_str(), MASTER_FOR_SLAVE);
		return false;
	}
	SocketIOService* service = (SocketIOService*)BNF::instance()->GetSessionPointer(shandle);
	__master_client.init(service);

	update_timer = BNF::instance()->CreateTimer(1000, &timer_class);

	return true;
}

bool myhaSlave::run()
{
	BNF::instance()->Init();

	LOG_DEBUG("start service myhaSlave(%d)", __group_id);
	printf(">>>> start service myhaSlave(%d)\n", __group_id);

	if (initService() == false)
		return false;

	LOG_DEBUG("myhaSlave ...... OK (with boost_%s)", BOOST_LIB_VERSION);
	printf("\t\t>>>> myhaSlave ...... OK (with boost_%s)\n", BOOST_LIB_VERSION);

	BNF::instance()->Run();

	BNF::instance()->Clear();

	LOG_DEBUG("end service myhaSlave(%d)", __group_id);
	printf(">>>> end service myhaSlave(%d)\n", __group_id);

	return true;
}

bool myhaSlave::loadScripts()
{
	return true;
}

void myhaSlave::Stop()
{
	endService();

	BNF::instance()->Stop();
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

		if (__master_client.service()->isValid() == true)
		{
			bool status = false;
			if (tcp_check == true && proc_check == true)
				status = true;

			// LOG_TRACE("Slave is %s.", (status == true) ? "OK" : "NOT OK");

			__master_client.deliver(MonitorAnnounce::ProcessStatus(2, status));
		}
	}
	else if (pSession->GetHandle() == connect_monitor_handle)
	{
		LOG_TRACE("--------------------------------------------------------------------------------");
// 		session_handle shandle = BNF::instance()->CreateConnect(Config::INI::Instance()->getMasterIP().c_str(), MASTER_FOR_SLAVE, &__master_client);
// 		if (shandle != SessionBase::INVALID_SESSION_HANDLE)
// 		{
// 			SocketIOService* service = (SocketIOService*)BNF::instance()->GetSessionPointer(shandle);
// 			__master_client.init(service);
// 		}
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
