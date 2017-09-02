#include "stdafx.h"

#include "MasterClientAccept.h"
#include "MasterClientService.h"
#include "monitorresponse.h"

MasterClientAccept::MasterClientAccept()
{
}

MasterClientAccept::~MasterClientAccept()
{
}

MasterClientAccept* MasterClientAccept::instance()
{
	static MasterClientAccept center;
	return & center;
}

void MasterClientAccept::operate(SocketIOService* service)
{
	MasterClientService* centerservice = new MasterClientService(service);
	service->SetUserData(centerservice);

	service->deliver( CLocalRequest::serverInfo( SERVER_TYPE_MONITOR, 0, 0 ) );
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
	ip_map_t::iterator it = __center_list.find(service->getGroupID());
	if (it == __center_list.end())
		return;

	__center_list.erase(it);
}

void MasterClientAccept::broadcast( Packet* packet )
{
	if ( __center_list.empty() )
		return;

	broadcast( Packet::SP(packet) );
}

void MasterClientAccept::broadcast( Packet::SP packet )
{
	if ( __center_list.empty() )
		return;

	BOOST_FOREACH( ip_map_t::value_type& p, __center_list )
	{
		MasterClientService* center = p.second;
		if ( center )
			center->deliver( packet );
	}
}
