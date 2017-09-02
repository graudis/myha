#include "stdafx.h"

#include "iniconfig.h"

#include "LogSystem.h"

#include "localrequest.h"
#include "MasterClientService.h"
#include "myhaMaster.h"
#include "network_util.h"

#include "ProcessCheck.h"

session_handle myhaMaster::server_command_handle = SessionBase::INVALID_SESSION_HANDLE;
TimerClass myhaMaster::timer_class;

void TimerClass::operate(SocketIOService* service)
{
	LOG_TRACE("");
	myhaMaster::processTimerSession((TimerSession*)service);
}

bool myhaMaster::config()
{
	return true;
}

bool myhaMaster::init()
{
	BNF::instance()->Init();

	session_handle handle;

	handle = BNF::instance()->CreateListen(getServiceIP(), MASTER_FOR_SLAVE, 0, MasterClientAccept::instance());
	if (handle == SessionBase::INVALID_SESSION_HANDLE)
	{
		LOG_ERROR("Can't create listen service['%s:%d'] for center.", getServiceIP(), MASTER_FOR_SLAVE);
		printf("[%s:%d	%s]	Can't create listen service['%s:%d'] for center.\n", __FILE__, __LINE__, __FUNCTION__, getServiceIP(), MASTER_FOR_SLAVE);
		return false;
	}

	myhaMaster::server_command_handle = BNF::instance()->CreateTimer(1000, &timer_class);

	return true;
}

bool myhaMaster::run()
{
	if (init() == false)
	{
		BNF::instance()->Clear();
		return false;
	}

	printf(">>>> start service Monitor(%s)\n", getServiceIP());
	printf(">>>> run service Monitor(%s)\n", getServiceIP());

	BNF::instance()->Run();

	BNF::instance()->Clear();

	printf(">>>> end service Monitor(%s)\n", getServiceIP());

	return true;
}

void myhaMaster::end()
{
}

void myhaMaster::processTimerSession(TimerSession* session)
{
	if (session->GetHandle() == myhaMaster::server_command_handle)
	{
		uint16_t port = 3306;

		bool tcp_check = TCPPortCheck(Config::INI::Instance()->getMasterIP(), port);
		bool proc_check = ProcessCheck();

		bool status = false;
		if (tcp_check == true && proc_check == true)
			status = true;

		LOG_TRACE("Master is %s.", (status == true) ? "OK" : "NOT OK");

		MasterClientService* center = MasterClientAccept::instance()->lookup(1);
		if (center != NULL)
		{
			center->deliver(MonitorAnnounce::ProcessStatus(1, status));
		}
	}
}
