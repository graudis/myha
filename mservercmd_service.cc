#include "stdafx.h"

#include "mservercmd_service.h"
#include "mservercmd.h"

MServerCmdService::MServerCmdService() :
    service_(NULL)
{
}

MServerCmdService::~MServerCmdService()
{
}

void MServerCmdService::init(rnSocketIOService* service, const tCHAR* daemonIP)
{
	service_	= service;
	daemonIP_	= daemonIP;
}

void MServerCmdService::clear()
{
	service_	= NULL;
}

void MServerCmdService::operate(rnSocketIOService* service)
{
	rnPacket::SP packet(service->GetMessage());
	if (packet == NULL)
	{
		CONNECT_LOG_INFO(server_session_info_, service->ip().c_str(), "$$$ disconnect reason[%s] $$$", service->GetErrorMessage().c_str());
		bnf::instance()->RemoveSession(service->GetHandle());
		MServerCmd::instance()->deleteService(daemonIP_.c_str());
		return;
	}

	switch(packet->group())
	{
	case PGROUP_LOCAL_REQUEST:
		{
			server_session_info_ = *((TServerInfo*)packet->data());
			service->deliver( CLocalResponse::serverInfo( SERVER_TYPE_MONITOR, 0, 0 ) );
		}
		break;
	default:
		{
			LOG_ERROR("unknown packet type (%d,%d)", packet->group(), packet->type());
		}
		break;
	}
}
