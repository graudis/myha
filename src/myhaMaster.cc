#include "stdafx.h"

#include "Config_INI.h"

#include "LogSystem.h"
#include "network_util.h"
#include "ProcessCheck.h"

#include "localrequest.h"

#include "MasterClientService.h"


#include "myhaMaster.h"

session_handle myhaMaster::server_command_handle = SessionBase::INVALID_SESSION_HANDLE;
TimerClass myhaMaster::timer_class;

bool myhaMaster::__mysql_ok;
bool myhaMaster::__connected;
bool myhaMaster::__slave_ok;
bool myhaMaster::__have_vip_master;
bool myhaMaster::__have_vip_slave;
UStatusChange myhaMaster::__status_old;
UStatusChange myhaMaster::__status_cur;

void TimerClass::operate(SocketIOService* service)
{
	myhaMaster::processTimerSession((TimerSession*)service);
}

bool myhaMaster::config()
{
	return true;
}

myhaMaster::myhaMaster()
{
	__status_old.reset();
	__status_cur.reset();
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

	server_command_handle = BNF::instance()->CreateTimer(1000, &timer_class);

	setMySqlOK(false);
	setConnected(false);
	setSlaveOK(false);
	setHaveVIPMaster(false);
	setHaveVIPSlave(false);

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

		if (status == true)
		{
			setMySqlOK(true);
		}
		else
		{
			setMySqlOK(false);
		}

		sendStatus(status);

		if (isConnected() == false)
		{
			printState();
			return;
		}

		__status_cur.statusMaster(isMySqlOK());
		__status_cur.statusSlave(isSlaveOK());

		if (__status_cur.status != __status_old.status)
		{
			LOG_TRACE("!!! Status change !!!");
			if (isMySqlOK() == true)
			{
				LOG_TRACE("");

				requestVIPDown();
				setHaveVIPSlave(false);
				VIPUp();
				setHaveVIPMaster(true);
			}
			else if (isMySqlOK() == false)
			{
				LOG_TRACE("");
				if (isSlaveOK() == true)
				{
					requestVIPUp();
					setHaveVIPSlave(true);
					VIPDown();
					setHaveVIPMaster(false);
				}
				else
				{
					requestVIPDown();
					setHaveVIPSlave(false);
					VIPDown();
					setHaveVIPMaster(false);
				}
			}
		}

		printState();
		__status_old = __status_cur;
	}
}

void myhaMaster::requestHaveVIP()
{
	MasterClientService* slave = MasterClientAccept::instance()->lookup(1);
	if (slave != NULL)
	{
		LOG_TRACE("deliver CLocalRequest::haveVIP");
		slave->deliver(CLocalRequest::haveVIP());
	}
}

void myhaMaster::requestVIPDown()
{
	MasterClientService* slave = MasterClientAccept::instance()->lookup(1);
	if (slave != NULL)
	{
		LOG_TRACE("deliver CLocalRequest::requestVIPDown");
		slave->deliver(CLocalRequest::requestVIPDown());
	}
}

void myhaMaster::requestVIPUp()
{
	MasterClientService* slave = MasterClientAccept::instance()->lookup(1);
	if (slave != NULL)
	{
		LOG_TRACE("deliver CLocalRequest::requestVIPUp");
		slave->deliver(CLocalRequest::requestVIPUp());
	}
}

void myhaMaster::VIPDown()
{
	std::string command;
	command = "ip addr del " + Config::INI::Instance()->getVirtualIP() + "/24 dev " + Config::INI::Instance()->getVirtualIPNIC() + " > /dev/null 2>&1";
	LOG_TRACE("command=[ '%s' ]", command.c_str());
	int32_t x = system(command.c_str());
}

void myhaMaster::VIPUp()
{
	std::string command;
	command = "ip addr add " + Config::INI::Instance()->getVirtualIP() + "/24 dev " + Config::INI::Instance()->getVirtualIPNIC() + " > /dev/null 2>&1";
	LOG_TRACE("command=[ '%s' ]", command.c_str());
	int32_t x = system(command.c_str());
}

void myhaMaster::sendStatus(bool status)
{
	MasterClientService* center = MasterClientAccept::instance()->lookup(1);
	if (center != NULL)
	{
		center->deliver(MonitorAnnounce::ProcessStatus(1, status));
	}
}

void myhaMaster::printState()
{
	std::string str;

	str = "Connected : ";
	if (isConnected() == true)
		str += "OK    ";
	else
		str += "NOT OK";

	str += " - Master state :: MySQL : ";
	if (isMySqlOK() == true)
		str += "OK    ";
	else
		str += "NOT OK";

	str += " - Slave : ";
	if (isSlaveOK() == true)
		str += "OK    ";
	else
		str += "NOT OK";

	str += " - VIP : ";

	switch (HaveVIP())
	{
	case 0:	str += "NONE";		break;
	case 1: str += "master";	break;
	case 2: str += "slave";		break;
	default: str += "!!! (" + HaveVIP() + std::string(")");
	}

	LOG_TRACE("%s", str.c_str());
}

int32_t myhaMaster::HaveVIP()
{
	if (haveVIPMaster() == false && haveVIPSlave() == false)
		return 0;

	if (haveVIPMaster() == true && haveVIPSlave() == false)
		return 1;

	if (haveVIPMaster() == false && haveVIPSlave() == true)
		return 2;

	if (haveVIPMaster() == true && haveVIPSlave() == true)
		return 3;

	return 4;
}
