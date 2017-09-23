#include "stdafx.h"

#include "network_util.h"
#include "Config_INI.h"

#include "localresponse.h"
#include "monitorresponse.h"

#include "MasterClient.h"
#include "myhaSlave.h"

MasterClient::MasterClient() :
	__service(NULL)
{
}

MasterClient::~MasterClient()
{
	__service = NULL;
}

bool MasterClient::init(SocketIOService* service, bool is_server_process_info)
{
	__service = service;

	LOG_DEBUG("CMonitor add %d", __service->GetHandle());

	if (is_server_process_info)
	{
		// 모니터가 혼자 내려갔다가 다시 올라왔기 때문에 서버들의 정보를 다시 보낸다
		myhaSlave::getServerSessionManager().sendConnectProcessInfo();
	}

	return true;
}

void MasterClient::clear()
{
	if (__service)
	{
		LOG_DEBUG("MasterClient delete %d", __service->GetHandle());
	}

	__service = NULL;

	LOG_TRACE("로그인 서버와 끊어졌다 다시 붙이자");
	myhaSlave::connect_monitor_handle = BNF::instance()->CreateTimer(100, &myhaSlave::getTimerClass());
}

void MasterClient::VIPDown()
{
	std::string command;
	command = "ip addr del " + Config::INI::Instance()->getVirtualIP() + "/24 dev " + Config::INI::Instance()->getVirtualIPNIC() + " > /dev/null 2>&1";
	int32_t x = system(command.c_str());
}

void MasterClient::VIPUp()
{
	std::string command;
	command = "ip addr add " + Config::INI::Instance()->getVirtualIP() + "/24 dev " + Config::INI::Instance()->getVirtualIPNIC() + " > /dev/null 2>&1";
	int32_t x = system(command.c_str());
}

void MasterClient::operate(SocketIOService* service)
{
	Packet::SP packet(service->GetMessage());
	if (packet == NULL)
	{
		LOG_INFO("$$$ disconnect reason[%s] $$$", service->GetErrorMessage().c_str());
		BNF::instance()->RemoveSession(service->GetHandle());
		clear();
		return;
	}

	// LOG_TRACE("packet[%3d:%3d]", packet->getGroup(), packet->getType());
	if (packet->getGroup() == PGROUP_MONITOR_ANNOUNCE)
	{
		switch (packet->getType())
		{
		case PTYPE_MONITOR_Process_Status:
		{
			MonitorAnnounce::TProcessStatus* data = reinterpret_cast<MonitorAnnounce::TProcessStatus*>(packet->data());

			if (data->mode == 1)
			{
				LOG_TRACE("Master is %s.", (data->status == true) ? "OK" : "NOT OK");
			}
			else if (data->mode == 2)
			{
				LOG_TRACE("Slave is %s.", (data->status == true) ? "OK" : "NOT OK");
			}
			else
			{
				LOG_TRACE("Something wrong! mode=[ %d ] status=[ %d ]", data->mode, data->status);
			}
		}
		break;
		}
	}
	else if (packet->getGroup() == PGROUP_LOCAL_REQUEST)
	{
		switch (packet->getType())
		{
		case PTYPE_LOCAL_SERVER_INFO:
		{
			__server_session_info = *((TServerInfo*)packet->data());
			service->deliver(CLocalResponse::serverInfo(SERVER_TYPE_SLAVE, myhaSlave::getGroupID(), 0));
		}
		break;
		case PTYPE_LOCAL_HAVE_VIP:
		{
			bool has_vip = checkVIP(Config::INI::Instance()->getVirtualIPNIC(), Config::INI::Instance()->getVirtualIP());
			service->deliver(CLocalResponse::hasVIP(has_vip));
		}
		break;
		case PTYPE_LOCAL_VIP_DOWN:
		{
			LOG_TRACE("VIP down");
			VIPDown();
		}
		break;
		case PTYPE_LOCAL_VIP_UP:
		{
			LOG_TRACE("VIP up");
			VIPUp();
		}
		break;
		}
	}
}
