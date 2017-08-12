#include "stdafx.h"

#include "mcenteraccept.h"
#include "mcenterservice.h"
#include "monitorresponse.h"

MCenterAccept::MCenterAccept()
{
	current_crs_count_ = 0;
	current_nhn_count_ = 0;
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

MCenterService* MCenterAccept::lookup(tSINT group_id)
{
	ip_map_t::iterator it = center_list_.find(group_id);
	return (it != center_list_.end()) ? it->second : NULL;
}

void MCenterAccept::createService(tSINT group_id, MCenterService* service)
{
	center_list_.insert(ip_map_t::value_type(group_id, service));
}

void MCenterAccept::deleteService(MCenterService* service)
{
	ip_map_t::iterator it = center_list_.find(service->getGroupID());
	if (it == center_list_.end())
		return;

	center_list_.erase(it);
}

void MCenterAccept::broadcast( rnPacket* packet )
{
	if ( center_list_.empty() )
		return;

	broadcast( rnPacket::SP(packet) );
}

void MCenterAccept::broadcast( rnPacket::SP packet )
{
	if ( center_list_.empty() )
		return;

	BOOST_FOREACH( ip_map_t::value_type& p, center_list_ )
	{
		MCenterService* center = p.second;
		if ( center )
			center->deliver( packet );
	}
}

void MCenterAccept::setCurrentConnectUser( tINT current_crs_count, tINT current_nhn_count )
{
	current_crs_count_ = current_crs_count;
	current_nhn_count_ = current_nhn_count;
}

void MCenterAccept::sendAllConnectInfo( rnSocketIOService* service )
{
	tINT info_size = center_list_.size();
	tINT packet_size = sizeof( MonitorResponse::TCurrentConnectUsers ) + ( sizeof( MonitorResponse::TGroupUsers ) * info_size );

	rnPacket* send_packet = new rnPacket( packet_size );
	send_packet->setGroupType(PGROUP_MONITOR_RESPONSE, PTYPE_MONITOR_CURRENT_CONNECT_USERS);
	MonitorResponse::TCurrentConnectUsers* data = reinterpret_cast<MonitorResponse::TCurrentConnectUsers*>(send_packet->data());
	data->group_count_ = info_size;

	tINT count = 0;
	BOOST_FOREACH( ip_map_t::value_type& p, center_list_ )
	{
		MCenterService* center = p.second;
		if ( center )
		{
			data->group_users_[count].group_id_ = center->getGroupID();
 			data->group_users_[count].total_login_users_ = center->getConnectUsers();
 			data->group_users_[count].crs_login_users_ = center->getCrsConnectUsers();
 			data->group_users_[count].nhn_login_users_ = center->getNhnConnectUsers();

//			UserCountInfo user_count = center->getMapConnectUsers();

// 			data->group_users_[count].total_map_users_ = user_count.crs_user_count_ + user_count.nhn_user_count_;
// 			data->group_users_[count].crs_map_users_ = user_count.crs_user_count_;
// 			data->group_users_[count].nhn_map_users_ = user_count.nhn_user_count_;

			count++;
		}

	}

	send_packet->setDataSizeWithoutHeader( packet_size );
	service->deliver( send_packet );
}
