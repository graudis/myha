#include "stdafx.h"

#include "localresponse.h"
#include "mcenterservice.h"
#include "mservermanager.h"
#include "connmonitorservice.h"
#include "mservercommandmanager.h"

#include "gmapsetting.h"
#include "myhaMaster.h"

MCenterService::MCenterService(rnSocketIOService* service) :
	service_(service)
{
	crs_connect_users_ = 0;
	nhn_connect_users_ = 0;

	login_limit_ = 0;

	memset( status_value_, 0, sizeof(status_value_) );
}

MCenterService::~MCenterService()
{
	rnPacket::SP alertPacket(MonitorResponse::daemonEndAlert( getProcessID(), service()->ip().c_str() ));
	// MServerManager::push_rnpacket_data(service_, alertPacket);

	{
		UserCountInfoMap::iterator begin = map_users_list_.begin();
		UserCountInfoMap::iterator end = map_users_list_.end();
		for( ; begin != end; )
		{
			UserCountInfoMap::iterator it = begin;
			begin++;

			map_users_list_.erase( it );
		}
	}

	{
		map_t::iterator begin = map_user_limit_list_.begin();
		map_t::iterator end = map_user_limit_list_.end();
		for( ; begin != end; )
		{
			map_t::iterator it = begin;
			begin++;

			map_user_limit_list_.erase( it );
		}
	}

	setConnectUsers( 0, 0 );
	service_ = NULL;
}

void MCenterService::setConnectUsers( tINT crs_count, tINT nhn_count )
{
	tINT new_crs_count = MCenterAccept::instance()->getCurrentCrsCount() - crs_connect_users_;
	new_crs_count += crs_count;

	tINT new_nhn_count = MCenterAccept::instance()->getCurrentCrsCount() - nhn_connect_users_;
	new_nhn_count += nhn_count;

	MCenterAccept::instance()->setCurrentConnectUser( new_crs_count, new_nhn_count );

	crs_connect_users_ = crs_count;
	nhn_connect_users_ = nhn_count;

//	LOG_TRACE( "setConnectUsers. %d, %d", crs_connect_users_, nhn_connect_users_ );
}

//void MCenterService::setMapConnectUsers( tINT map_id, tINT channel_id, tINT crs_user_count, tINT nhn_user_count )
//{
//	UserCountInfoMap::iterator it = map_users_list_.find( MAKE_MAP_CHANNEL_ID( map_id, channel_id ) );
//	if ( it == map_users_list_.end() )
//	{
//		UserCountInfo count_info;
//		count_info.crs_user_count_ = crs_user_count;
//		count_info.nhn_user_count_ = nhn_user_count;
//
//		map_users_list_.insert( UserCountInfoMap::value_type( MAKE_MAP_CHANNEL_ID( map_id, channel_id ), count_info ) );
//
////		LOG_TRACE( "map. %d, %d", crs_user_count, nhn_user_count );
//		return;
//	}
//
//	it->second.crs_user_count_ = crs_user_count;
//	it->second.nhn_user_count_ = nhn_user_count;
//
////	LOG_TRACE( "map. %d, %d", crs_user_count, nhn_user_count );
//}

//UserCountInfo MCenterService::getMapConnectUsers()
//{
//	UserCountInfo user_count;
//	user_count.crs_user_count_ = 0;
//	user_count.nhn_user_count_ = 0;
//
//	BOOST_FOREACH( UserCountInfoMap::value_type& p, map_users_list_ )
//	{
//		user_count.crs_user_count_ += p.second.crs_user_count_;
//		user_count.nhn_user_count_ += p.second.nhn_user_count_;
//	}
//
//	return user_count;
//}

//tINT MCenterService::getMapConnectLimit( tINT map_id )
//{
//	map_t::iterator it = map_user_limit_list_.find( map_id );
//	if ( it == map_user_limit_list_.end() )
//		return -1;
//
//	return it->second;
//}

//void MCenterService::setMapConnectLimit( tINT map_id, tINT map_connect_limit )
//{
//	map_t::iterator it = map_user_limit_list_.find( map_id );
//	if ( it == map_user_limit_list_.end() )
//		return;
//
//	it->second = map_connect_limit;
//}

//void MCenterService::addMapConnectLimit( tINT map_id )
//{
//	map_t::iterator it = map_user_limit_list_.find( map_id );
//	if ( it != map_user_limit_list_.end() )
//		return;
//
//	gMapSetting* map_setting = gMapSetting::lookup( map_id );
//	if ( map_setting == NULL )
//	{
//		LOG_ERROR( "Can't found map script[%d]", map_id );
//		return;
//	}
//
//	map_user_limit_list_.insert( map_t::value_type( map_id, map_setting->getMaxUserPerChannel() ) );
//}

void MCenterService::operate(rnSocketIOService* service)
{
	rnPacket::SP packet(service->GetMessage());
	if (packet == NULL)
	{
		CONNECT_LOG_INFO(server_session_info_, service->ip().c_str(), "$$$ disconnect reason[%s] $$$", service->GetErrorMessage().c_str());
		bnf::instance()->RemoveSession(service->GetHandle());
		MCenterAccept::instance()->deleteService(this);

		delete this;
		return;
	}

	switch(packet->group())
	{
	case PGROUP_LOCAL_REQUEST:
		{
			switch ( packet->type() )
			{
			case PTYPE_LOCAL_POST_EVENT:
			case PTYPE_LOCAL_RECALL_POST_EVENT:
			case PTYPE_LOCAL_SEND_SCHEDULE_MAIL:
				MCenterAccept::instance()->broadcast( packet );
				break;
			case PTYPE_LOCAL_ERROR_ETIMEDOUT:
				handleErrorETIemdOut( packet );
				break;
			}
		}
		break;
	case PGROUP_LOCAL_RESPONSE:
		{
			switch ( packet->type() )
			{
			case PTYPE_LOCAL_SERVER_INFO:
				{
					server_session_info_ = *((TServerInfo*)packet->data());

					LOG_TRACE("server_session_info_.id_=[ %d ]", server_session_info_.id_);

					MCenterAccept::instance()->createService((tSINT)server_session_info_.id_, this);

					rnPacket::SP sp_packet = rnPacket::SP ( MonitorRequest::realtimeEventUpdate( server_session_info_.id_ ) );
					// MServerCommandManager::push_rnpacket_data( sp_packet );
				}
				break;
			case PTYPE_LOCAL_CHANGE_SERVER_SETTING:
				{
					CLocalResponse::TChangeServerSetting* recv_data = (CLocalResponse::TChangeServerSetting*)packet->data();
					kill_mob_exp_rate_ = recv_data->kill_mob_exp_rate_;
				}
				break;
			default:
				{
					LOG_ERROR("Invalid packet group[%d] type[%d]", packet->group(), packet->type());
				}
				break;
			}
		}
		break;
	case PGROUP_MONITOR_RESPONSE:
		{
			switch( packet->type() )
			{
			case PTYPE_MONITOR_DAEMON_START_ALERT:
				{
					MonitorResponse::TDaemonStartAlert* data = reinterpret_cast<MonitorResponse::TDaemonStartAlert*>(packet->data());

					if (data->sort_ == SERVER_TYPE_CENTER)
					{
						MCenterService* service = MCenterAccept::instance()->lookup(data->group_id_);
						if (service != NULL)
						{
							setGroupID(data->group_id_);
							setProcessID(data->process_id_);
						}
					}
					else if ( data->sort_ == SERVER_TYPE_GCONTAINER )
					{
//						addMapConnectLimit( data->id_ );
					}

					tUINT id = MAKE_MAP_CHANNEL_ID( data->id_, data->channelid_ );
					map_t::iterator it = channel_starting_list_.find( id );
					if ( it != channel_starting_list_.end() )
						channel_starting_list_.erase( id );

					// MServerManager::push_rnpacket_data(service, packet);
				}
				break;
			case PTYPE_MONITOR_DAEMON_END_ALERT:
				// MServerManager::push_rnpacket_data(service, packet);
				break;
			case PTYPE_MONITOR_CHANNEL_DAEMON_RUN:
				{
					MonitorResponse::TChannelDaemonRun* data = reinterpret_cast<MonitorResponse::TChannelDaemonRun*>(packet->data());
					tUINT id = MAKE_MAP_CHANNEL_ID( data->map_id_, data->channel_id_ );
					map_t::iterator it = channel_starting_list_.find( id );
					if ( it != channel_starting_list_.end() )
					{
						tUINT limit_time = time( 0 ) - it->second;
						LOG_ERROR( "already starting game channel map id[%d] channel id[%d] limit time[%u]", data->map_id_, data->channel_id_, limit_time );
						if ( limit_time < 10 )
							return;

						channel_starting_list_.erase( id );
					}

					channel_starting_list_.insert( map_t::value_type( id, time( 0 ) ) );

					// MServerManager::push_rnpacket_data(service, packet);
				}
				break;
			default:
				{
					LOG_ERROR("Invalid packet group[%d] type[%d]", packet->group(), packet->type());
				}
				break;
			}
		}
		break;
	case PGROUP_MONITOR_ANNOUNCE:
		{
			switch (packet->type() )
			{
			case PTYPE_MONITOR_CURRENT_CONNECT_USERS:
				{
					MonitorAnnounce::TCurrentConnectUsers* data = reinterpret_cast<MonitorAnnounce::TCurrentConnectUsers*>( packet->data() );

					setConnectUsers( data->total_crs_users_, data->total_nhn_users_ );
				}
				break;
			case PTYPE_MONITOR_MAP_CONNECT_USERS:
				{
//					MonitorAnnounce::TMapConnectUsers* data = reinterpret_cast<MonitorAnnounce::TMapConnectUsers*>( packet->data() );
//
//					setMapConnectUsers( data->map_id_, data->channel_id_, data->crs_user_count_, data->nhn_user_count_ );
				}
				break;
			case PTYPE_MONITOR_ABNORMAL_EXP:
				MConnMonitorManager::instance()->deliverAll( packet );
				break;
			case PTYPE_MONITOR_Process_Status:
				{
					MonitorAnnounce::TProcessStatus* data = reinterpret_cast<MonitorAnnounce::TProcessStatus*>(packet->data());

					if (data->mode == 1)
					{
						LOG_TRACE("Master is %s.", (data->status == true) ? "OK" :"NOT OK");
					}
					else if (data->mode == 2)
					{
						LOG_TRACE("Slave is %s.", (data->status == true) ? "OK" :"NOT OK");
					}
					else
					{
						LOG_TRACE("Something wrong! mode=[ %d ] status=[ %d ]", data->mode, data->status);
					}
				}
				break;
			default:
				{
					LOG_ERROR("Invalid packet group[%d] type[%d]", packet->group(), packet->type());
				}
				break;
			}
		}
		break;
	default:
		{
			LOG_ERROR("Invalid packet group[%d] type[%d]", packet->group(), packet->type());
		}
		break;
	}
}

void MCenterService::setGroupID( tINT group_id )
{
	group_id_ = group_id;
	login_limit_ = 10;
}


void MCenterService::handleErrorETIemdOut( rnPacket::SP& packet )
{
//	CLocalRequest::TErrorETimedOut* recv_data = (CLocalRequest::TErrorETimedOut*)packet->data();
}
