#include "stdafx.h"

#include "cmonitor.h"
#include "myhaSlave.h"

#include "localresponse.h"
#include "monitorresponse.h"

CMonitor::CMonitor() :
	__service(NULL)
{
}

CMonitor::~CMonitor()
{
	__service = NULL;
}

bool CMonitor::init(rnSocketIOService* service, bool is_server_process_info)
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

void CMonitor::clear()
{
	if (__service)
	{
		LOG_DEBUG("CMonitor delete %d", __service->GetHandle());
	}

	__service = NULL;

	myhaSlave::connect_monitor_handle = bnf::instance()->CreateTimer(100, &myhaSlave::getTimerClass());
}

void CMonitor::operate(rnSocketIOService* service)
{
	rnPacket::SP packet(service->GetMessage());
	if (packet == NULL)
	{
		bnf::instance()->RemoveSession(service->GetHandle());
		clear();
		return;
	}

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

			LOG_TRACE("Center::groupid()=[ %d ]", myhaSlave::getGroupID());

			service->deliver(CLocalResponse::serverInfo(SERVER_TYPE_CENTER, myhaSlave::getGroupID(), 0));
		}
		break;
		}
	}
}
