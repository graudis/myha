#include "stdafx.h"

#include "network_util.h"

#include "localresponse.h"

#include "myhaMaster.h"

#include "MasterClientService.h"

MasterClientService::MasterClientService(SocketIOService* service) :
	__service(service)
{
	__login_limit = 0;
}

MasterClientService::~MasterClientService()
{
	__service = NULL;
}

void MasterClientService::operate(SocketIOService* service)
{
	Packet::SP packet(service->GetMessage());
	if (packet == NULL)
	{
		LOG_TRACE("getGroupID()=[ %d ]", getGroupID());
		BNF::instance()->RemoveSession(service->GetHandle());
		MasterClientAccept::instance()->deleteService(this);

		delete this;
		return;
	}

	// LOG_TRACE("packet[%3d:%3d]", packet->getGroup(), packet->getType());
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
			MasterClientAccept::instance()->createService(__server_session_info.id, this);

			myhaMaster::requestHaveVIP();
		}
		break;
		case PTYPE_LOCAL_HAVE_VIP:
		{
			CLocalResponse::THaveVIP slave_has_vip = *((CLocalResponse::THaveVIP*)packet->data());
			myhaMaster::setHaveVIPSlave(slave_has_vip.have_vip);
			LOG_TRACE("slave has VIP %d.", slave_has_vip.have_vip);

			bool master_have_vip = false;
			master_have_vip = checkVIP(Config::INI::Instance()->getVirtualIPNIC(), Config::INI::Instance()->getVirtualIP());
			myhaMaster::setHaveVIPMaster(master_have_vip);
			LOG_TRACE("master has VIP %d", master_have_vip);
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

			if (data->sort == SERVER_TYPE_SLAVE)
			{
				MasterClientService* service = MasterClientAccept::instance()->lookup(data->group_id);
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
				// LOG_TRACE("Master is %s.", (data->status == true) ? "OK" : "NOT OK");
			}
			else if (data->mode == 2)
			{
				// LOG_TRACE("Slave is %s.", (data->status == true) ? "OK" : "NOT OK");
				myhaMaster::setSlaveOK(data->status);
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

bool MasterClientService::deliver(Packet* packet)
{
	if (__service == NULL)
	{
		return false;
	}

	return deliver(Packet::SP(packet));
}

bool MasterClientService::deliver(Packet::SP packet)
{
	if (__service == NULL)
	{
		LOG_TRACE("__service == NULL");
		return false;
	}

	__service->deliver(packet);

	return true;
}

void MasterClientService::setGroupID(int16_t group_id)
{
	__group_id = group_id;
	__login_limit = 10;
}
