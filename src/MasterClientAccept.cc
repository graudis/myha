#include "stdafx.h"

#include "monitorresponse.h"

#include "myhaMaster.h"
#include "MasterClientAccept.h"
#include "MasterClientService.h"

MasterClientAccept::MasterClientAccept()
{
}

MasterClientAccept::~MasterClientAccept()
{
}

MasterClientAccept* MasterClientAccept::instance()
{
	static MasterClientAccept center;
	return &center;
}

void MasterClientAccept::operate(SocketIOService* service)
{
	MasterClientService* client_service = new MasterClientService(service);
	service->SetUserData(client_service);

	myhaMaster::setConnected(true);

	service->deliver(CLocalRequest::serverInfo(SERVER_TYPE_MASTER, 0, 0));
}

MasterClientService* MasterClientAccept::lookup(int16_t group_id)
{
	ip_map_t::iterator it = __center_list.find(group_id);
	return (it != __center_list.end()) ? it->second : NULL;
}

void MasterClientAccept::createService(int16_t group_id, MasterClientService* service)
{
	__center_list.insert(ip_map_t::value_type(group_id, service));
}

void MasterClientAccept::deleteService(MasterClientService* service)
{
	LOG_TRACE("__center_list.size()=[ %u ]", __center_list.size());
	LOG_TRACE("service->getGroupID()=[ %d ]", service->getGroupID());
	ip_map_t::iterator it = __center_list.find(service->getGroupID());
	if (it == __center_list.end())
		return;
	LOG_TRACE("__center_list.size()=[ %u ]", __center_list.size());
	myhaMaster::setConnected(false);
	__center_list.erase(it);
}

void MasterClientAccept::broadcast(Packet* packet)
{
	if (__center_list.empty())
		return;

	broadcast(Packet::SP(packet));
}

void MasterClientAccept::broadcast(Packet::SP packet)
{
	if (__center_list.empty())
		return;

	BOOST_FOREACH(ip_map_t::value_type& p, __center_list)
	{
		MasterClientService* center = p.second;
		if (center)
			center->deliver(packet);
	}
}
