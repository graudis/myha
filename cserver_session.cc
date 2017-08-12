#include "stdafx.h"
#include "cserver_session.h"
#include "ccenter.h"
#include "myhaSlave.h"

#include "localrequest.h"
#include "localannounce.h"
#include "localresponse.h"
#include "monitorresponse.h"
#include "monitorannounce.h"

#include "gameresponse.h"
#include "gameannounce.h"

#include "gmapsetting.h"


//////////////////////////////////////////////////////////////////////////
// CServerSession
CServerSession::CServerSession(CCenter* center, TServerInfo* server_info) : center_(center)
{
	server_session_info_ = *server_info;
	process_id_ = 0;

	LOG_INFO("new CServerSession. type: %d - %s, id: %d, channel: %d", getType(), getServerName(getType()), getId(), getChannel());
}

CServerSession::~CServerSession()
{
	LOG_INFO("delete CServerSession. type: %d - %s, id: %d, channel: %d", getType(), getServerName(getType()), getId(), getChannel());
}

tBOOL CServerSession::packetHandler(rnPacket::SP& packet)
{
	return cFALSE;
}

void CServerSession::addConnectionInfo(TListenInfo* connection_info)
{
	listen_info_list_.push_back(*connection_info);
}

void CServerSession::sendConectionInfo(CServerSession* server_session)
{
	std::list<TListenInfo>::iterator it = listen_info_list_.begin();
	std::list<TListenInfo>::iterator end = listen_info_list_.end();

	for (; it != end; it++)
	{
		server_session->sendConectionInfo(this, &(*it));
	}
}

void CServerSession::sendConectionInfo(CServerSession* server_session, TListenInfo* listen_info)
{
	// server_session의 listen_info가 this가 접속해야하는 서버라면 아이피,포트를 알려준다.
	if (getType() == listen_info->for_type_)
	{
		deliver(CLocalAnnounce::connectionInfo(server_session->getType(), server_session->getId(),
			server_session->getChannel(), listen_info->ip_, listen_info->port_));
	}
}

void CServerSession::deliver(rnPacket::SP& packet)
{
	center_->deliver(packet);
}

void CServerSession::deliver(rnPacket* packet)
{
	center_->deliver(packet);
}

const tCHAR* CServerSession::getIp()
{
	return center_->getIp();
}


//////////////////////////////////////////////////////////////////////////
// CProxySession
CProxySession::CProxySession(CCenter* center, TServerInfo* server_info) :
	CServerSession(center, server_info)
{
	crs_user_count_ = 0;
	nhn_user_count_ = 0;
}

CProxySession::~CProxySession()
{
	tINT old_crs_count = crs_user_count_;
	tINT old_nhn_count = nhn_user_count_;

	crs_user_count_ = 0;
	nhn_user_count_ = 0;

	myhaSlave::getServerSessionManager().setTotalCrsUserCount(old_crs_count, crs_user_count_);
	myhaSlave::getServerSessionManager().setTotalNhnUserCount(old_nhn_count, nhn_user_count_);

	myhaSlave::getMonitor().deliver(MonitorAnnounce::currentConnectUsers(myhaSlave::getServerSessionManager().getTotalCrsUserCount(),
		myhaSlave::getServerSessionManager().getTotalNhnUserCount()));
}

tBOOL CProxySession::packetHandler(rnPacket::SP& packet)
{
	if (CServerSession::packetHandler(packet) == cTRUE)
		return cTRUE;

	switch (packet->group())
	{
	case PGROUP_LOCAL_REQUEST:
	{
		switch (packet->type())
		{
		case PTYPE_LOCAL_USER_COUNT:
		{
			CLocalRequest::TPUserCount* recv_data = (CLocalRequest::TPUserCount*)packet->data();

			tINT old_crs_count = crs_user_count_;
			tINT old_nhn_count = nhn_user_count_;

			crs_user_count_ = recv_data->crs_user_count_;
			nhn_user_count_ = recv_data->nhn_user_count_;

			myhaSlave::getServerSessionManager().setTotalCrsUserCount(old_crs_count, crs_user_count_);
			myhaSlave::getServerSessionManager().setTotalNhnUserCount(old_nhn_count, nhn_user_count_);

			if (myhaSlave::getServerSessionManager().getTotalUserCount() > myhaSlave::getusers_count_max())
				myhaSlave::setusers_count_max(myhaSlave::getServerSessionManager().getTotalUserCount());

			if (myhaSlave::getServerSessionManager().getTotalCrsUserCount() > myhaSlave::getcrs_users_count_max())
				myhaSlave::setcrs_users_count_max(myhaSlave::getServerSessionManager().getTotalCrsUserCount());

			if (myhaSlave::getServerSessionManager().getTotalNhnUserCount() > myhaSlave::getnhn_users_count_max())
				myhaSlave::setnhn_users_count_max(myhaSlave::getServerSessionManager().getTotalNhnUserCount());

			// 현재 proxy server에 접속한 유저의 수를 login server로 전송
// 					TLoginCount	data;
// 					data.proxy_id_ = getId();
// 					data.proxy_users_ = getUserCount();
// 					Center::getLogin().deliver( CLocalRequest::CLoginSession( data ) );
			myhaSlave::getMonitor().deliver(MonitorAnnounce::currentConnectUsers(myhaSlave::getServerSessionManager().getTotalCrsUserCount(),
				myhaSlave::getServerSessionManager().getTotalNhnUserCount()));
		}
		break;
		}
	}
	break;
	}

	return cTRUE;
}

//////////////////////////////////////////////////////////////////////////
// CHcashSession
CHcashSession::CHcashSession(CCenter* center, TServerInfo* server_info) : CServerSession(center, server_info)
{
}

CHcashSession::~CHcashSession()
{

}

tBOOL CHcashSession::packetHandler(rnPacket::SP& packet)
{
	if (CServerSession::packetHandler(packet) == cTRUE)
		return cTRUE;

	switch (packet->group())
	{
	case PGROUP_LOCAL_REQUEST:
	{
		switch (packet->type())
		{
		case PTYPE_LOCAL_END:
			LOG_INFO("receive end packet from hcash");
			LOG_INFO("send signal to self with SIGTERM");
			kill(getpid(), SIGTERM);
			break;
		}
	}
	break;
	case PGROUP_LOCAL_ANNOUNCE:
	{
		switch (packet->type())
		{
		case PTYPE_LOCAL_DELETE_ACTOR:
			myhaSlave::getServerSessionManager().deliver(SERVER_TYPE_DODB, packet);
			break;
		}
	}
	break;
	}

	return cTRUE;
}


//////////////////////////////////////////////////////////////////////////
// CContainerSession
CContainerSession::CContainerSession(CCenter* center, TServerInfo* server_info) : CServerSession(center, server_info),
current_connect_users_(0),
is_channel_map_(cFALSE),
max_user_count_(0)
{
}

CContainerSession::~CContainerSession()
{
	myhaSlave::getInstanceMapManager().removeMap(getId(), getChannel());

	myhaSlave::getInstanceCoolTimeManager().removeMap(this);

	myhaSlave::getMonitor().deliver(MonitorAnnounce::mapConnectUsers(getId(), getChannel(), 0, 0));
}

tBOOL CContainerSession::packetHandler(rnPacket::SP& packet)
{
	if (CServerSession::packetHandler(packet) == cTRUE)
		return cTRUE;

	switch (packet->group())
	{
	case PGROUP_MEMBER_ANNOUNCE:
	{
		switch (packet->type())
		{
		case PTYPE_MEMBER_MESSAGE_SPAWN_MOB:
		case PTYPE_MEMBER_MESSAGE_DEAD_MOB:
		case PTYPE_MEMBER_MISSION_MT_MATCH_RESULT_NOTICE:
			myhaSlave::getServerSessionManager().deliver(SERVER_TYPE_PROXY, packet);
			break;
		}
	}
	break;
	case PGROUP_LOCAL_REQUEST:
	{
		switch (packet->type())
		{
		case PTYPE_LOCAL_OPEN_MISSION_MAP:
			handleLocalOpenMissionMap(packet);
			break;
		case PTYPE_LOCAL_CLOSE_MISSION_MAP:
			handleLocalCloseMissionMap(packet);
			break;
		case PTYPE_LOCAL_JOIN_MISSION_MAP_REQ:
			handleLocalJoinMissionMapReq(packet);
			break;
		case PTYPE_LOCAL_REMOVE_QUEUE_MISSION_MAP:
			handleLocalRemoveQueueMissionMap(packet);
			break;
		case PTYPE_LOCAL_CREATE_INSTANCE:
			handleCreateInstance(packet);
			break;
		case PTYPE_LOCAL_DESTROY_INSTANCE:
			handleDestroyInstance(packet);
			break;
		case PTYPE_LOCAL_BIND_INSTANCE:
			handleBindInstance(packet);
			break;
		case PTYPE_LOCAL_UNBIND_USER_INSTANCE:
			handleUnBindUserInstance(packet);
			break;
		case PTYPE_LOCAL_UNBIND_PARTY_INSTANCE:
			handleUnBindPartyInstance(packet);
			break;
		case PTYPE_LOCAL_QUERY_BOUND_INSTANCE:
			handleQueryBoundInstance(packet);
			break;
		case PTYPE_LOCAL_POST_EVENT:
		case PTYPE_LOCAL_RECALL_POST_EVENT:
		case PTYPE_LOCAL_SEND_SCHEDULE_MAIL:
			myhaSlave::getMonitor().deliver(packet);
			break;
		case PTYPE_LOCAL_CENTER_INMAP:
			handleCenterInMap(packet);
			break;
		case PTYPE_LOCAL_CENTER_OUTMAP:
			handleCenterOutMap(packet);
			break;
		case PTYPE_LOCAL_RESET_INSTANCE:
			handleResetInstance(packet);
			break;
		case PTYPE_LOCAL_RESET_INSTANCE_ALL:
			handleResetInstanceAll(packet);
			break;
		case PTYPE_LOCAL_INSTANCE_COOLTIME_LIST:
			handleInstanceCooltimeList(packet);
			break;
		case PTYPE_LOCAL_RESET_INSTANCE_COOLTIME_ALL:
			handleResetInstanceCooltimeAll(packet);
			break;
		case PTYPE_LOCAL_START_DESTROY_INSTANCE:
			handleStartDestroyInstance(packet);
			break;
		case PTYPE_LOCAL_CANCEL_DESTROY_INSTANCE:
			handleCancelDestroyInstance(packet);
			break;
		case PTYPE_LOCAL_RAID_MOB_REGEN_STATUS:
			handleRaidMObRegenStatus(packet);
			break;
		case PTYPE_LOCAL_ERROR_ETIMEDOUT:
			myhaSlave::getMonitor().deliver(packet);
			break;
		}
	}
	break;
	case PGROUP_LOCAL_RESPONSE:
	{
		switch (packet->type())
		{
		case PTYPE_LOCAL_CHANNEL_USER_COUNT:
		{
			CLocalResponse::TChannelUserCount* data = reinterpret_cast<CLocalResponse::TChannelUserCount*>(packet->data());

			setCurrentConnectUsers(data->crs_user_count_ + data->nhn_user_count_);

			myhaSlave::getMonitor().deliver(MonitorAnnounce::mapConnectUsers(getId(), getChannel(), data->crs_user_count_, data->nhn_user_count_));

			if (isChannelMap() == cTRUE)
			{
				tINT run_channel_id = 0;

				if (myhaSlave::getServerSessionManager().canNextChannleMapRun(getId(), run_channel_id) == cTRUE)
				{
					myhaSlave::getMonitor().deliver(MonitorResponse::channelDaemonRun(myhaSlave::groupid(), getId(), run_channel_id));
				}
			}
		}
		break;
		case PTYPE_LOCAL_JOIN_MISSION_MAP_RES:
			handleLocalJoinMissionMapRes(packet);
			break;
		}
	}
	break;
	case PGROUP_LOCAL_ANNOUNCE:
	{
		switch (packet->type())
		{
		case PTYPE_LOCAL_MISSION_MAP_ANNOUNCE:
			handleLocalMissionMapAnnounce(packet);
			break;
		case PTYPE_LOCAL_PRODUCE_SUCCESS_ANNOUNCE:
			myhaSlave::getServerSessionManager().deliver(SERVER_TYPE_PROXY, packet);
			break;
		case PTYPE_LOCAL_CHANNEL_MAP_INFO:
		{
			CLocalAnnounce::TChannelMapInfo* data = reinterpret_cast<CLocalAnnounce::TChannelMapInfo*>(packet->data());
			setChannelMapInfo(data->is_channel_map_, data->max_user_count_);
		}
		break;
		}
	}
	break;
	case PGROUP_MONITOR_ANNOUNCE:
	{
		switch (packet->type())
		{
		case PTYPE_MONITOR_ABNORMAL_EXP:
			myhaSlave::getMonitor().deliver(packet);
			break;
		case PTYPE_MONITOR_Process_Status:
		{
			MonitorAnnounce::TProcessStatus* data = reinterpret_cast<MonitorAnnounce::TProcessStatus*>(packet->data());
			LOG_TRACE("mode=[ %d ] status=[ %d ]", data->mode, data->status);
		}
		break;
		}
	}
	break;
	}

	return cTRUE;
}

void CContainerSession::sendConectionInfo(CServerSession* server_session, TListenInfo* listen_info)
{
	if (server_session->getType() == SERVER_TYPE_GMOB)
	{
		if (getId() == server_session->getId() && getChannel() == server_session->getChannel())
		{
			if (getType() == listen_info->for_type_)
			{
				deliver(CLocalAnnounce::connectionInfo(server_session->getType(),
					server_session->getId(), server_session->getChannel(), listen_info->ip_, listen_info->port_));
			}
		}
	}
	else
	{
		CServerSession::sendConectionInfo(server_session, listen_info);
	}
}

void CContainerSession::setChannelMapInfo(tBOOL is_channel_map, tINT max_user_count)
{
	is_channel_map_ = is_channel_map;
	max_user_count_ = max_user_count;
}

void CContainerSession::handleLocalOpenMissionMap(rnPacket::SP& packet)
{
	CLocalRequest::TOpenMissionMap* recv_data = (CLocalRequest::TOpenMissionMap*)packet->data();
	myhaSlave::getServerSessionManager().deliverToContainer(recv_data->map_id_, packet);
}

void CContainerSession::handleLocalCloseMissionMap(rnPacket::SP& packet)
{
	CLocalRequest::TCloseMissionMap* recv_data = (CLocalRequest::TCloseMissionMap*)packet->data();
	myhaSlave::getServerSessionManager().deliverToContainer(recv_data->map_id_, packet);
}

void CContainerSession::handleLocalJoinMissionMapReq(rnPacket::SP& packet)
{
	CLocalRequest::TJoinMissionMapReq* recv_data = (CLocalRequest::TJoinMissionMapReq*)packet->data();

	// 이동 가능 채널 얻음
	CContainerSession* container_session = myhaSlave::getServerSessionManager().getCanJoinMIssioMap(recv_data->to_map_id_);
	if (container_session == NULL)
	{
		// 이동 가능 채널 없음
		deliver(CLocalResponse::joinMissionMapRes(recv_data->to_map_id_, recv_data->to_channel_id_,
			recv_data->from_map_id_, recv_data->from_channel_id_, MISSIONMAP_STATE_MAP_IS_FULL, recv_data->actor_id_));
		return;
	}

	//	LOG_TRACE( "push queue mission. %d, %d", container_session->getId(), container_session->getChannel() );

		// actor_id로 큐 함
	container_session->addMissionMapWaitQueue(recv_data->actor_id_);

	container_session->deliver(packet);
}

void CContainerSession::handleLocalJoinMissionMapRes(rnPacket::SP& packet)
{
	CLocalResponse::TJoinMissionMapRes* recv_data = (CLocalResponse::TJoinMissionMapRes*)packet->data();

	if (recv_data->state_ != 0)
	{
		//		LOG_TRACE( "pop queue mission. %d, %d", getId(), getChannel() );
		wait_actor_set_.erase(recv_data->actor_id_);
	}

	CServerSession* ssesion = myhaSlave::getServerSessionManager().findServerSession(SERVER_TYPE_GCONTAINER, recv_data->to_map_id_, recv_data->to_channel_id_);
	if (ssesion != NULL)
	{
		ssesion->deliver(packet);
	}
}

void CContainerSession::handleLocalRemoveQueueMissionMap(rnPacket::SP& packet)
{
	// 맵 입장 성공 또는 큐 시간 초과할 때 gc에서 보내 줌
	CLocalRequest::TRemoveQueueMissionMap* recv_data = (CLocalRequest::TRemoveQueueMissionMap*)packet->data();

	//	LOG_TRACE( "pop queue mission. %d, %d", getId(), getChannel() );
	wait_actor_set_.erase(recv_data->actor_id_);
}

void CContainerSession::handleLocalMissionMapAnnounce(rnPacket::SP& packet)
{
	CLocalAnnounce::TMissionMapAnnounce* recv_data = (CLocalAnnounce::TMissionMapAnnounce*)packet->data();

	mission_map_info_.map_id_ = recv_data->map_id_;
	mission_map_info_.status_ = recv_data->status_;
	mission_map_info_.min_level_ = recv_data->min_level_;
	mission_map_info_.max_level_ = recv_data->max_level_;
	mission_map_info_.max_players_ = recv_data->max_players_;

	// 모든 채널의 상태가 같은 상태로 변경 될 때 전체 유저에게 미션맵 상태를 알려준다
	if (myhaSlave::getServerSessionManager().isAllMissionMapStatus(getId(), mission_map_info_.status_) == cTRUE)
	{
		LOG_DEBUG("send MissionMapStatus. map: %d, status: %d", getId(), mission_map_info_.status_);
		myhaSlave::getServerSessionManager().deliver(SERVER_TYPE_GCONTAINER, packet);
		myhaSlave::getServerSessionManager().deliver(SERVER_TYPE_PROXY, packet);
	}
}

void CContainerSession::handleCreateInstance(rnPacket::SP& packet)
{
	CLocalRequest::TCreateInstance* recv_data = (CLocalRequest::TCreateInstance*)packet->data();

	myhaSlave::getInstanceMapManager().createInstance(recv_data->map_id_, recv_data->channel_id_, recv_data->instance_id_,
		recv_data->difficulty_);
}

void CContainerSession::handleDestroyInstance(rnPacket::SP& packet)
{
	CLocalRequest::TDestroyInstance* recv_data = (CLocalRequest::TDestroyInstance*)packet->data();

	myhaSlave::getInstanceMapManager().destroyInstance(recv_data->map_id_, recv_data->channel_id_, recv_data->instance_id_);
}

void CContainerSession::handleBindInstance(rnPacket::SP& packet)
{
	CLocalRequest::TBindInstance* recv_data = (CLocalRequest::TBindInstance*)packet->data();

	// 귀속
	tBOOL user_bind = cFALSE;
	InstanceSave* instance_save = myhaSlave::getInstanceMapManager().bindInstance(recv_data->map_id_, recv_data->channel_id_,
		recv_data->instance_id_, recv_data->user_id_, recv_data->party_id_, &user_bind);
	if (instance_save == NULL)
	{
		LOG_ERROR("handleBindInstance error. map_id: %d, channel_id: %d, instance_id: %d, user_id: %d, party_id: %d",
			recv_data->map_id_, recv_data->channel_id_, recv_data->instance_id_, recv_data->user_id_, recv_data->party_id_);
		return;
	}

	if (user_bind == cTRUE)
	{
		// 쿨타임 저장
		// tINT bind_cooltime = 0;
		// gMapSetting* map_settting = gMapSetting::lookup(instance_save->map_id_);

		// 인던 맵 사용하지만, 일반 던전이 아닌 쿨타임 저장을 하지 않는다
		// if (map_settting->isMissionMap() == cFALSE &&
		// 	map_settting->isTournamentBattleMap() == cFALSE &&
		// 	map_settting->isTowerMap() == cFALSE)
		// {
		// 	if (map_settting != NULL)
		// 		bind_cooltime = map_settting->getInstanceBindTime();
		// 
		// 	Center::getInstanceCoolTimeManager().addCooltime(recv_data->user_id_, instance_save->map_id_, instance_save->channel_id_,
		// 		instance_save->difficulty_, instance_save->instance_id_, bind_cooltime);
		// }
	}
}

void CContainerSession::handleUnBindUserInstance(rnPacket::SP& packet)
{
	CLocalRequest::TUnBindUserInstance* recv_data = (CLocalRequest::TUnBindUserInstance*)packet->data();

	myhaSlave::getInstanceMapManager().unbindInstanceUser(recv_data->map_id_, recv_data->channel_id_, recv_data->difficulty_, recv_data->instance_id_,
		recv_data->user_id_);
}

void CContainerSession::handleUnBindPartyInstance(rnPacket::SP& packet)
{
	CLocalRequest::TUnBindPartyInstance* recv_data = (CLocalRequest::TUnBindPartyInstance*)packet->data();

	myhaSlave::getInstanceMapManager().unbindInstanceParty(recv_data->map_id_, recv_data->channel_id_, recv_data->difficulty_, recv_data->instance_id_,
		recv_data->party_id_);
}

void CContainerSession::handleQueryBoundInstance(rnPacket::SP& packet)
{
	CLocalRequest::TQueryBoundInstance* recv_data = (CLocalRequest::TQueryBoundInstance*)packet->data();

	CLocalResponse::TQueryBoundInstance send_data;
	send_data.reason_ = INSTANCE_NO_ERROR;
	send_data.map_id_ = recv_data->map_id_;
	send_data.user_id_ = recv_data->user_id_;
	send_data.x_ = recv_data->x_;
	send_data.z_ = recv_data->z_;
	send_data.use_portal_id_ = recv_data->use_portal_id_;
	send_data.difficulty_ = recv_data->difficulty_;
	send_data.party_bind_ = cFALSE;
	send_data.move_state_ = recv_data->move_state_;

	InstanceCooltime::InstanceInfo* cooltime_instance = myhaSlave::getInstanceCoolTimeManager().getCooltimeInfo(recv_data->user_id_,
		recv_data->map_id_, recv_data->difficulty_);
	if (cooltime_instance != NULL && cooltime_instance->expired_time_ > myhaSlave::getGlobalTime())
	{
		tBOOL party_bind = cFALSE;
		InstanceSave* instance_save = myhaSlave::getInstanceMapManager().findBoundInstanceSave(recv_data->map_id_,
			recv_data->difficulty_, recv_data->user_id_, 0, &party_bind);
		if (instance_save != NULL)
		{
			// 인던 입장 가능
			send_data.channel_id_ = instance_save->channel_id_;
			send_data.instance_id_ = instance_save->instance_id_;
			send_data.difficulty_ = instance_save->difficulty_;
		}
		else
		{
			// 인던 입장 불가
			send_data.reason_ = INSTANCE_ERROR_HAVE_COOLTIME;
		}
	}
	else
	{
		tBOOL party_bind = cFALSE;

		InstanceSave* instance_save = myhaSlave::getInstanceMapManager().findBoundInstanceSave(recv_data->map_id_,
			recv_data->difficulty_, recv_data->user_id_, recv_data->party_id_, &party_bind);
		if (instance_save != NULL)
		{
			send_data.channel_id_ = instance_save->channel_id_;
			send_data.instance_id_ = instance_save->instance_id_;
			send_data.difficulty_ = instance_save->difficulty_;
		}
		else
		{
			// 인원 수가 적은 채널을 gc에게 알려줌
			CContainerSession* container_session = myhaSlave::getServerSessionManager().getMinUserCountMap(recv_data->map_id_);
			if (container_session != NULL)
			{
				send_data.channel_id_ = container_session->getChannel();
				send_data.instance_id_ = 0;
			}
		}

		send_data.party_bind_ = party_bind;
	}

	deliver(CLocalResponse::queryBoundInstance(send_data));

}

void CContainerSession::handleResetInstance(rnPacket::SP& packet)
{
	CLocalRequest::TResetInstance* recv_data = (CLocalRequest::TResetInstance*)packet->data();

	tBOOL use_item = cFALSE;

	// 쿨타임이 남아 있으면 - 아이템으로 초기화 가능한지 체크
	InstanceCooltime::InstanceInfo* cooltime_instance = myhaSlave::getInstanceCoolTimeManager().getCooltimeInfo(recv_data->user_id_,
		recv_data->map_id_, recv_data->difficulty_);

	if (cooltime_instance != NULL && cooltime_instance->expired_time_ > myhaSlave::getGlobalTime())
	{
		if (recv_data->remain_reset_count_ == 0)
		{
			deliver(CLocalResponse::resetInstance(INSTANCE_ERROR_RESET_COUNT_LIMIT, recv_data->user_id_, 0, 0, 0));
			return;
		}

		if (recv_data->inven_index_ == -1)
		{
			deliver(CLocalResponse::resetInstance(INSTANCE_ERROR_RESET_NOT_HAVE_ITEM, recv_data->user_id_, 0, 0, 0));
			return;
		}

		// 쿨타임 리셋
		myhaSlave::getInstanceCoolTimeManager().resetCooltime(recv_data->user_id_, recv_data->map_id_, recv_data->difficulty_);
		use_item = cTRUE;
	}

	// 쿨타임 끝나있으면 인던 귀속 해제 시켜줌
	InstanceSave* instance_save = myhaSlave::getInstanceMapManager().findBoundInstanceSave(recv_data->map_id_,
		recv_data->difficulty_, recv_data->user_id_, 0);
	if (instance_save != NULL)
	{
		myhaSlave::getInstanceMapManager().unbindInstanceUser(instance_save->map_id_, instance_save->channel_id_,
			instance_save->difficulty_, instance_save->instance_id_, recv_data->user_id_);
	}

	deliver(CLocalResponse::resetInstance(INSTANCE_NO_ERROR, recv_data->user_id_, recv_data->map_id_, recv_data->difficulty_,
		use_item));
}

void CContainerSession::handleResetInstanceAll(rnPacket::SP& packet)
{
	CLocalRequest::TResetInstanceAll* recv_data = (CLocalRequest::TResetInstanceAll*)packet->data();

	myhaSlave::getInstanceMapManager().unbindInstanceAll(recv_data->user_id_);

	myhaSlave::getInstanceCoolTimeManager().sendCooltimeData(recv_data->user_id_, this);
}

void CContainerSession::handleInstanceCooltimeList(rnPacket::SP& packet)
{
	CLocalRequest::TGetInstanceCooltime* recv_data = (CLocalRequest::TGetInstanceCooltime*)packet->data();

	myhaSlave::getInstanceCoolTimeManager().sendCooltimeData(recv_data->user_id_, this);
}

/*
void CContainerSession::handleResetInstanceCooltime( rnPacket::SP& packet )
{
	CLocalRequest::TResetInstanceCooltime* recv_data = (CLocalRequest::TResetInstanceCooltime*)packet->data();

	if( Center::getInstanceCoolTimeManager().resetCooltimeAll( recv_data->user_id_, recv_data->map_id_, recv_data->difficulty_ ) == cFALSE )
	{
		// 실패
		deliver( CLocalResponse::resetInstanceCooltime( 1, recv_data->user_id_, recv_data->map_id_, recv_data->difficulty_ ) );
		return;
	}

	// 성공
	deliver( CLocalResponse::resetInstanceCooltime( 0, recv_data->user_id_, recv_data->map_id_, recv_data->difficulty_ ) );
}
*/

void CContainerSession::handleResetInstanceCooltimeAll(rnPacket::SP& packet)
{
	CLocalRequest::TResetInstanceCooltimeAll* recv_data = (CLocalRequest::TResetInstanceCooltimeAll*)packet->data();

	myhaSlave::getInstanceCoolTimeManager().resetCooltimeAll(recv_data->user_id_);
}

void CContainerSession::handleStartDestroyInstance(rnPacket::SP& packet)
{
	CLocalRequest::TStartDestroyInstance* recv_data = (CLocalRequest::TStartDestroyInstance*)packet->data();

	myhaSlave::getInstanceCoolTimeManager().startUnloadTime(recv_data->map_id_, recv_data->channel_id_,
		recv_data->instance_id_, recv_data->unload_time_);
}

void CContainerSession::handleCancelDestroyInstance(rnPacket::SP& packet)
{
	CLocalRequest::TCancelDestroyInstance* recv_data = (CLocalRequest::TCancelDestroyInstance*)packet->data();

	myhaSlave::getInstanceCoolTimeManager().cancelUnloadTime(recv_data->map_id_, recv_data->channel_id_,
		recv_data->instance_id_);
}

void CContainerSession::handleCenterInMap(rnPacket::SP& packet)
{
	CLocalRequest::TCenterInMap* recv_data = (CLocalRequest::TCenterInMap*)packet->data();

	myhaSlave::getInstanceCoolTimeManager().inMap(recv_data->user_id_, this);

	deliver(CLocalResponse::centerInMap(0, recv_data->user_id_));
}

void CContainerSession::handleCenterOutMap(rnPacket::SP& packet)
{
	CLocalRequest::TCenterOutMap* recv_data = (CLocalRequest::TCenterOutMap*)packet->data();

	myhaSlave::getInstanceCoolTimeManager().outMap(recv_data->user_id_, this);
}
void CContainerSession::handleRaidMObRegenStatus(rnPacket::SP& packet)
{
	// 	CLocalRequest::TRaidMobRegenStatus* recv_data = (CLocalRequest::TRaidMobRegenStatus*)packet->data();
	// 	Center::getMobServerManager().sendRaidMobRegenStatus( recv_data->actor_id_, this );
}


//////////////////////////////////////////////////////////////////////////
// CMobSession
CMobSession::CMobSession(CCenter* center, TServerInfo* server_info) : CServerSession(center, server_info)
{
}

CMobSession::~CMobSession()
{
}

tBOOL CMobSession::packetHandler(rnPacket::SP& packet)
{
	if (CServerSession::packetHandler(packet) == cTRUE)
		return cTRUE;

	switch (packet->group())
	{
	case PGROUP_GAME_RESPONSE:
	{
		switch (packet->type())
		{
		case PTYPE_GAME_RESPAWN_UNIQUE_MOB:
			handleGameResponse_RespawnUniqueMob(packet);
			break;
		}
	}
	break;
	case PGROUP_GAME_ANNOUNCE:
	{
		switch (packet->type())
		{
		case PTYPE_GAME_DEAD_UNIQUE_MOB:
			handleGameAnnounce_DeadUniqueMob(packet);
			break;
		case PTYPE_GAME_READY_MOB_SERVER:
			handleGameAnnounce_ReadyMobServer(packet);
			break;
		}
	}
	break;
	}

	return cTRUE;
}

void CMobSession::sendConectionInfo(CServerSession* server_session, TListenInfo* listen_info)
{
	if (server_session->getType() == SERVER_TYPE_GCONTAINER)
	{
		if (getId() == server_session->getId() && getChannel() == server_session->getChannel())
		{
			if (getType() == listen_info->for_type_)
			{
				deliver(CLocalAnnounce::connectionInfo(server_session->getType(),
					server_session->getId(), server_session->getChannel(), listen_info->ip_, listen_info->port_));

				LOG_INFO("send cinfo. %s. type: %s, id: %d, ch: %d", getServerName(getType()),
					getServerName(server_session->getType()), server_session->getId(), server_session->getChannel());
			}
		}
	}
	else
	{
		CServerSession::sendConectionInfo(server_session, listen_info);
	}
}

void CMobSession::handleGameResponse_RespawnUniqueMob(rnPacket::SP& packet)
{
	// GameResponse::TRespawnUniqueMob* recv_data = (GameResponse::TRespawnUniqueMob*)packet->data();
	// Center::getMobServerManager().responseRegenMob( getId(), getChannel(), recv_data->reason_, recv_data->guid_ );
}

void CMobSession::handleGameAnnounce_DeadUniqueMob(rnPacket::SP& packet)
{
	// 	GameAnnounce::TDeadUniqueMob* recv_data = (GameAnnounce::TDeadUniqueMob*)packet->data();
	// 	Center::getMobServerManager().recvedDeadMob( getId(), getChannel(), recv_data->guid_ );
}

void CMobSession::handleGameAnnounce_ReadyMobServer(rnPacket::SP& packet)
{
	// 	Center::getMobServerManager().addServer( this );
}


//////////////////////////////////////////////////////////////////////////
// CDodbSession
CDodbSession::CDodbSession(CCenter* center, TServerInfo* server_info) : CServerSession(center, server_info)
{
}

CDodbSession::~CDodbSession()
{
}

tBOOL CDodbSession::packetHandler(rnPacket::SP& packet)
{
	if (CServerSession::packetHandler(packet) == cTRUE)
		return cTRUE;

	switch (packet->group())
	{
	case PGROUP_LOCAL_RESPONSE:
	{
		switch (packet->type())
		{
		case PTYPE_LOCAL_CHANGE_SERVER_SETTING:
			myhaSlave::getMonitor().deliver(packet);
			break;
		}
	}
	break;
	case PGROUP_LOCAL_ANNOUNCE:
	{
		switch (packet->type())
		{
			//XX 잭팟 coin broadcasting (center)
		case PTYPE_LOCAL_JACKPOT_MONEY:
			myhaSlave::getServerSessionManager().deliver(SERVER_TYPE_PROXY, packet);
			break;
			//XX 잭팟 click result broadcasting (center)
		case PTYPE_LOCAL_JACKPOT_RESULT_BROADCASTING:
			myhaSlave::getServerSessionManager().deliver(SERVER_TYPE_PROXY, packet);
			break;
		case PTYPE_LOCAL_SERVER_INIT:
			myhaSlave::setServerInit();
			break;
		}
	}
	break;
	}

	return cTRUE;
}


//////////////////////////////////////////////////////////////////////////
// CDodbSession
CChatSession::CChatSession(CCenter* center, TServerInfo* server_info) : CServerSession(center, server_info)
{
}

CChatSession::~CChatSession()
{
}

tBOOL CChatSession::packetHandler(rnPacket::SP& packet)
{
	if (CServerSession::packetHandler(packet) == cTRUE)
		return cTRUE;

	switch (packet->group())
	{
	case PGROUP_LOCAL_REQUEST:
	{
		switch (packet->type())
		{
		case PTYPE_LOCAL_LOGIN_CHAT_NEW:
		{
			CLocalRequest::TLoginChatNew* data = reinterpret_cast<CLocalRequest::TLoginChatNew*>(packet->data());

			STRNCPY(ip_, data->ip_, sizeof(ip_));
			port_ = data->port_;

			// 					Center::getLogin().deliver( packet );
		}
		break;
		}
	}
	break;
	}

	return cTRUE;
}

