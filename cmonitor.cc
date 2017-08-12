#include "stdafx.h"

#include "cmonitor.h"
#include "myhaSlave.h"

#include "localresponse.h"
#include "monitorrequest.h"
#include "monitorresponse.h"

CMonitor::CMonitor() :
	service_(NULL),
	realtime_event_status_(cTRUE),
	realtime_event_map_count_(0),
	realtime_event_info_(NULL)
{
}

CMonitor::~CMonitor()
{
	service_ = NULL;
}

tBOOL CMonitor::init(rnSocketIOService* service, tBOOL is_server_process_info)
{
	service_ = service;

	LOG_DEBUG( "CMonitor add %d", service_->GetHandle() );

	if ( is_server_process_info )
	{
		// 모니터가 혼자 내려갔다가 다시 올라왔기 때문에 서버들의 정보를 다시 보낸다
		myhaSlave::getServerSessionManager().sendConnectProcessInfo();
	}

	return cTRUE;
}

void CMonitor::clear()
{
	if(service_)
	{
		LOG_DEBUG( "CMonitor delete %d", service_->GetHandle() );
	}

	service_ = NULL;

	myhaSlave::connect_monitor_handle_ = bnf::instance()->CreateTimer(100, &myhaSlave::getTimerClass());
}

void CMonitor::sendConnectProcessInfo( tINT type, tINT id, tINT channel_id, tINT process_id, const tCHAR* ip )
{
	deliver(MonitorResponse::daemonStartAlert( myhaSlave::groupid(), type, id, channel_id, process_id, ip ) );
}

void CMonitor::sendAllUserBan()
{
	myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_PROXY, MonitorRequest::allUserBan( myhaSlave::groupid() ) );
}

void CMonitor::setRealtimeEvent( tBOOL realtime_event_status, tINT map_count, TRealTimePercent* event_info )
{
	realtime_event_status_ = realtime_event_status;

	if ( realtime_event_info_ != NULL )
	{
		for ( tINT i = 0; i < realtime_event_map_count_; i++ )
		{
			LOG_INFO( "old realtime_event map[%d] percent[%d], [%d], [%d], [%d]", realtime_event_info_[i].map_id_,
				realtime_event_info_[i].event_percent_[REALTIME_EVENT_EXP_PERCENT],
				realtime_event_info_[i].event_percent_[REALTIME_EVENT_COIN_PERCENT],
				realtime_event_info_[i].event_percent_[REALTIME_EVENT_FAME_PERCENT],
				realtime_event_info_[i].event_percent_[REALTIME_EVENT_JACKPOT_PERCENT]);
		}

		delete [] realtime_event_info_;
		realtime_event_info_ = NULL;
	}
	else
	{
		LOG_INFO( "realtime_event is null" );
	}

	if ( map_count <= 0 )
	{
		LOG_INFO( "realtime_event is reset" );
	}

	realtime_event_map_count_ = map_count;

	if ( map_count > 0 )
	{
		realtime_event_info_ = new TRealTimePercent[realtime_event_map_count_];

		memcpy( realtime_event_info_, event_info, sizeof( TRealTimePercent ) * realtime_event_map_count_ );
	}

	for ( tINT i = 0; i < realtime_event_map_count_; i++ )
	{
		LOG_INFO( "new realtime_event map[%d] percent[%d], [%d], [%d], [%d]", realtime_event_info_[i].map_id_,
			realtime_event_info_[i].event_percent_[REALTIME_EVENT_EXP_PERCENT],
			realtime_event_info_[i].event_percent_[REALTIME_EVENT_COIN_PERCENT],
			realtime_event_info_[i].event_percent_[REALTIME_EVENT_FAME_PERCENT],
			realtime_event_info_[i].event_percent_[REALTIME_EVENT_JACKPOT_PERCENT]);
	}

	rnPacket::SP sp_packet = rnPacket::SP ( CLocalRequest::realtimeEventUpdate( realtime_event_map_count_, realtime_event_info_ ) );
	myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_GCONTAINER, sp_packet );
}

void CMonitor::operate(rnSocketIOService* service)
{
	rnPacket::SP packet(service->GetMessage());
	if (packet == NULL)
	{
		CONNECT_LOG_INFO(server_session_info_, service->ip().c_str(), "$$$ disconnect reason[%s] $$$", service->GetErrorMessage().c_str());
		bnf::instance()->RemoveSession(service->GetHandle());
		clear();
		return;
	}

	if (packet->group() == PGROUP_MONITOR_ANNOUNCE)
	{
		switch (packet->type())
		{
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
		}
	}
	else if (packet->group() == PGROUP_LOCAL_REQUEST)
	{
		switch(packet->type())
		{
		case PTYPE_LOCAL_SERVER_INFO:
			{
				server_session_info_ = *((TServerInfo*)packet->data());

				LOG_TRACE("Center::groupid()=[ %d ]", myhaSlave::groupid());

				service->deliver( CLocalResponse::serverInfo( SERVER_TYPE_CENTER, myhaSlave::groupid(), 0 ) );
				service->deliver(MonitorResponse::daemonStartAlert(
					myhaSlave::groupid(), SERVER_TYPE_CENTER, 0, 0, getpid(), myhaSlave::ip_data()->getPrivateAddress()));
			}
			break;
		case PTYPE_LOCAL_REALTIME_EVENT_UPDATE:
			{
				CLocalRequest::TRealtimeEventUpdate* data = reinterpret_cast<CLocalRequest::TRealtimeEventUpdate*>(packet->data());

				if ( realtime_event_status_ == cFALSE )
				{
					LOG_INFO( "realtime_event_status_[%d]", realtime_event_status_ );
					return;
				}

				setRealtimeEvent( cTRUE, data->map_count_, data->event_info_ );
			}
			break;
		case PTYPE_LOCAL_UPDATE_LOGIN_CONNECT_COUNT:
			{
// 				CLocalRequest::TUpdateLoginConnectCount* data = reinterpret_cast<CLocalRequest::TUpdateLoginConnectCount*>( packet->data() );
// 				Center::getLogin().setLoginLimit( data->limit_count_ );
// 				Center::getLogin().deliver(CLocalRequest::CLoginLimit(data->limit_count_, Center::proxy_max()));
				//Center::getLogin().sendInit();
			}
			break;
		case PTYPE_LOCAL_UPDATE_MAP_CONNECT_COUNT:
			{
				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_GCONTAINER, packet );
				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_PROXY, packet );
			}
			break;
		case PTYPE_LOCAL_ADMIN_FORCE_LOGOUT:
			{
				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_HCASH, packet );
				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_PROXY, packet );
				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_GCONTAINER, packet );
				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_DODB, packet );
				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_CHAT, packet );
			}
			break;
		case PTYPE_LOCAL_CHAT_BAN:
			{
				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_DODB, packet );
			}
			break;
		case PTYPE_LOCAL_SERVICE_EXPIRE_TIME:
			{
				CLocalRequest::TServiceExpireTime* data = reinterpret_cast<CLocalRequest::TServiceExpireTime*>( packet->data() );

				if ( myhaSlave::service_expire_time_handle_ != SessionBase::INVALID_SESSION_HANDLE )
					bnf::instance()->RemoveSession(myhaSlave::service_expire_time_handle_);

				myhaSlave::service_expire_time_handle_ = bnf::instance()->CreateTimer( ( data->expire_time_ + 2 ) * 1000, &myhaSlave::timer_class_ );

				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_PROXY, packet );
			}
			break;
		case PTYPE_LOCAL_BILLING_SERVICE_LOCK:
			{
				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_PROXY, packet );
			}
			break;
		case PTYPE_LOCAL_CHANGE_SERVER_SETTING:
			myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_DODB, packet );
			break;
		case PTYPE_LOCAL_POST_EVENT:
		case PTYPE_LOCAL_RECALL_POST_EVENT:
		case PTYPE_LOCAL_SEND_SCHEDULE_MAIL:
			myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_DODB, packet );
			break;
		case PTYPE_LOCAL_RELOAD_CREATE_LIMIT:
			{
				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_GCONTAINER, packet );
				myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_DODB, packet );
			}
			break;
		case PTYPE_LOCAL_UPDATE_EVENT_LIST:
			LOG_INFO("Received PTYPE_LOCAL_UPDATE_EVENT_LIST");
			myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_HCASH, packet );
			break;
		}
	}
	else if (packet->group() == PGROUP_MONITOR_REQUEST)
	{
		switch(packet->type())
		{
		case PTYPE_MONITOR_USER_LOCK:
		case PTYPE_MONITOR_USER_DATA_SYNC:
		case PTYPE_MONITOR_SEND_MAIL_ALERT:
		case PTYPE_MONITOR_ALL_USER_BAN:
		case PTYPE_MONITOR_USE_NPROTECT_AUTH:
		case PTYPE_MONITOR_AUTOUSER_BAN:
			myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_PROXY, packet );
			break;
		}
	}
	else if (packet->group() == PGROUP_CHAT_ANNOUNCE)
	{
		switch(packet->type())
		{
		case PTYPE_CHAT_GM_PUBLIC_NOTIFY:
			{
				if ( myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_CHAT, packet ) == cFALSE )
				{
					packet->setGroupType( PGROUP_MEMBER_ANNOUNCE, PTYPE_MEMBER_GM_PUBLIC_NOTIFY );
					myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_PROXY, packet );
				}
			}
			break;
		case PTYPE_CHAT_GM_PRIVATE_NOTIFY:
			myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_CHAT, packet );
			break;
		}
	}
}
