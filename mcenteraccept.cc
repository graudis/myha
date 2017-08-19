#include "stdafx.h"

#include "mcenteraccept.h"
#include "mcenterservice.h"
#include "monitorresponse.h"

MCenterAccept::MCenterAccept()
{
}

MCenterAccept::~MCenterAccept()
{
}

MCenterAccept* MCenterAccept::instance()
{
	static MCenterAccept center;
	return & center;
}

void MCenterAccept::operate(rnSocketIOService* service)
{
	MCenterService* centerservice = new MCenterService(service);
	service->SetUserData(centerservice);

	service->deliver( CLocalRequest::serverInfo( SERVER_TYPE_MONITOR, 0, 0 ) );
}

MCenterService* MCenterAccept::lookup(int16_t group_id)
{
	ip_map_t::iterator it = __center_list.find(group_id);
	return (it != __center_list.end()) ? it->second : NULL;
}

void MCenterAccept::createService(int16_t group_id, MCenterService* service)
{
	__center_list.insert(ip_map_t::value_type(group_id, service));
}

void MCenterAccept::deleteService(MCenterService* service)
{
	ip_map_t::iterator it = __center_list.find(service->getGroupID());
	if (it == __center_list.end())
		return;

	__center_list.erase(it);
}

void MCenterAccept::broadcast( rnPacket* packet )
{
	if ( __center_list.empty() )
		return;

	broadcast( rnPacket::SP(packet) );
}

void MCenterAccept::broadcast( rnPacket::SP packet )
{
	if ( __center_list.empty() )
		return;

	BOOST_FOREACH( ip_map_t::value_type& p, __center_list )
	{
		MCenterService* center = p.second;
		if ( center )
			center->deliver( packet );
	}
}
