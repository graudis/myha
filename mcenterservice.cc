#include "stdafx.h"

#include "localresponse.h"
#include "mcenterservice.h"

#include "myhaMaster.h"

MCenterService::MCenterService(rnSocketIOService* service) :
	__service(service)
{
	__login_limit = 0;
}

MCenterService::~MCenterService()
{
	__service = NULL;
}

void MCenterService::operate(rnSocketIOService* service)
{
	rnPacket::SP packet(service->GetMessage());
	if (packet == NULL)
	{
		bnf::instance()->RemoveSession(service->GetHandle());
		MCenterAccept::instance()->deleteService(this);

		delete this;
		return;
	}

	switch (packet->getGroup())
	{
	case PGROUP_LOCAL_RESPONSE:
	{
		switch (packet->getType())
		{
		case PTYPE_LOCAL_SERVER_INFO:
		{
			__server_session_info = *((TServerInfo*)packet->data());

			LOG_TRACE("server_session_info_.id_=[ %d ]", __server_session_info.id);

			MCenterAccept::instance()->createService((int16_t)__server_session_info.id, this);

			// rnPacket::SP sp_packet = rnPacket::SP ( MonitorRequest::realtimeEventUpdate( server_session_info_.id_ ) );
			// MServerCommandManager::push_rnpacket_data( sp_packet );
		}
		break;
		default:
		{
			LOG_ERROR("Invalid packet group[%d] type[%d]", packet->getGroup(), packet->getType());
		}
		break;
		}
	}
	break;
	case PGROUP_MONITOR_RESPONSE:
	{
		switch (packet->getType())
		{
		case PTYPE_MONITOR_DAEMON_START_ALERT:
		{
			MonitorResponse::TDaemonStartAlert* data = reinterpret_cast<MonitorResponse::TDaemonStartAlert*>(packet->data());

			if (data->sort == SERVER_TYPE_CENTER)
			{
				MCenterService* service = MCenterAccept::instance()->lookup(data->group_id);
				if (service != NULL)
				{
					setGroupID(data->group_id);
					setProcessID(data->process_id);
				}
			}
		}
		break;
		default:
		{
			LOG_ERROR("Invalid packet group[%d] type[%d]", packet->getGroup(), packet->getType());
		}
		break;
		}
	}
	break;
	case PGROUP_MONITOR_ANNOUNCE:
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
		default:
		{
			LOG_ERROR("Invalid packet group[%d] type[%d]", packet->getGroup(), packet->getType());
		}
		break;
		}
	}
	break;
	default:
	{
		LOG_ERROR("Invalid packet group[%d] type[%d]", packet->getGroup(), packet->getType());
	}
	break;
	}
}

void MCenterService::setGroupID(int32_t group_id)
{
	__group_id = group_id;
	__login_limit = 10;
}
