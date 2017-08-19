#include "stdafx.h"

#include "Config_INI.h"

#include "logsystem.h"

#include "localrequest.h"
#include "mcenterservice.h"
#include "myhaMaster.h"
#include "network_util.h"

#include "ProcessCheck.h"

session_handle		myhaMaster::server_command_handle = SessionBase::INVALID_SESSION_HANDLE;
TimerClass			myhaMaster::timer_class;

void TimerClass::operate(rnSocketIOService* service)
{
	myhaMaster::processTimerSession((TimerSession*)service);
}

bool myhaMaster::config()
{
	return true;
}

bool myhaMaster::init()
{
	bnf::instance()->Init();

	session_handle handle;

	handle = bnf::instance()->CreateListen(getServiceIP(), MONITOR_FOR_CENTER, 0, MCenterAccept::instance());
	if (handle == SessionBase::INVALID_SESSION_HANDLE)
	{
		LOG_ERROR("Can't create listen service['%s:%d'] for center.", getServiceIP(), MONITOR_FOR_CENTER);
		printf("[%s:%d	%s]	Can't create listen service['%s:%d'] for center.\n", __FILE__, __LINE__, __FUNCTION__,	getServiceIP(), MONITOR_FOR_CENTER);
		return false;
	}

	myhaMaster::server_command_handle = bnf::instance()->CreateTimer(1000, &timer_class);

	return true;
}

bool myhaMaster::run()
{
	if (init() == false)
	{
		bnf::instance()->Clear();
		return false;
	}

	printf(">>>> start service Monitor(%s)\n", getServiceIP());
	printf(">>>> run service Monitor(%s)\n", getServiceIP());

	bnf::instance()->Run();

	bnf::instance()->Clear();

	printf(">>>> end service Monitor(%s)\n", getServiceIP());

	return true;
}

void myhaMaster::end()
{
}

void myhaMaster::processTimerSession( TimerSession* PSession )
{
	if (PSession->GetHandle() == myhaMaster::server_command_handle)
	{
		uint16_t port = 3306;

		bool tcp_check = TCPPortCheck(Config::INI::Instance()->getMasterIP(), port);
		bool proc_check = ProcessCheck();

		bool status = false;
		if (tcp_check == true && proc_check == true)
			status = true;

		LOG_TRACE("Master is %s.", (status == true) ? "OK" :"NOT OK");

		MCenterService* center = MCenterAccept::instance()->lookup(1);
		if (center != NULL)
		{
			center->deliver(MonitorAnnounce::ProcessStatus(1, status));
		}
	}
}
