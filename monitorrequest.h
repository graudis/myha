#pragma once

#define DAEMON_RUN_STRING_MAX	1024

namespace MonitorRequest
{
#pragma pack(push, 1)

	// web
	struct TWebUserLock
	{
		tINT	group_id_;
		tINT	user_index_;
	};

	struct TWebDaemonRun
	{
		tSINT	runType_;		// 데몬들의 그룹 아이디 0(login,bank),1(agent),3(game),4(game channel)
		tSINT	groupID_;		// 데몬 그룹 아이디
		tINT	typeID_;		// proxy 번호 or 맵 번호
		tINT	channelID_;		// 채널 아이디
	};

	struct TWebUserDataSync
	{
		tINT	group_id_;
		tINT	user_index_;
	};

	struct TWebSendMailAlert
	{
		tINT	group_id_;
		tINT	user_index_;
		tINT	actor_index_;
	};

	struct TWebServiceLock
	{
		tBYTE	is_lock_;
	};

	struct TWebAllUserBan
	{
		tBYTE	group_id_;		// 0이면 모든 서버
	};

	struct TWebNotify
	{
		tBYTE	group_id_;		// 0이면 모든 서버
		tBYTE	message_count_;
		tCHAR	message_[0];
	};

	struct TWebPrivateNotify
	{
		tBYTE	group_id_;
		tCHAR	actor_name_[STATUS_NAME_MAX];
		tBYTE	message_count_;
		tCHAR	message_[0];
	};

	struct TWebRefreshPacketVersion
	{
		tINT	packet_version_;
	};

	struct TWebRealtimeEventUpdate
	{
		tINT	group_id_;
		tINT	event_info_[REALTIME_EVENT_MAX];
	};

	struct TWebLoginConnectCountInfo
	{
		tINT	group_id_;
	};

	struct TWebUpdateLoginConnectCount
	{
		tINT	group_id_;
		tINT	limit_count_;
	};

	struct TWebMapConnectCountInfo
	{
		tINT	group_id_;
		tINT	map_id_;
	};

	struct TWebUpdateMapConnectCount
	{
		tINT	group_id_;
		tINT	map_id_;
		tINT	limit_count_;
	};

	struct TWebForceLogout
	{
		tINT	group_id_;
		tINT	force_level_;	// 0 - bank만 정리, 1 - 모든 서버에 전송
		tINT	user_index_;
		tINT	actor_index_;
	};

	struct TWebChatBan
	{
		tINT	group_id_;
		tCHAR	actor_name_[STATUS_NAME_MAX];
		tINT	chat_ban_limit_time_;
	};

	struct TWebServiceExpireTime
	{
		tBYTE	group_id_;		// 0이면 모든 서버
		tINT	expire_time_;	// 종료되는 시간
	};

	struct TWebServiceGroupLock
	{
		tBYTE	group_id_;
		tBYTE	is_lock_;
	};

	struct TWebBillingServiceLock
	{
		tBYTE	is_lock_;
	};

	struct TWebAutoUserBan
	{
		tINT	group_id_;
		tINT	user_index_;
	};

	struct TWebAddServerCommand
	{
		tUINT	command_no_;
	};

	struct TWebDelServerCommand
	{
		tUINT	command_no_;
	};

	struct TWebUpdateServerCommand
	{
		tUINT	command_no_;
	};

	// local
	struct TDaemonRun
	{
		tCHAR	execCommand_[DAEMON_RUN_STRING_MAX];
	};

	struct TUserLock
	{
		tINT	user_index_;
	};

	struct TUserDataSync
	{
		tINT	user_index_;
	};

	struct TSendMailAlert
	{
		tINT	user_index_;
		tINT	actor_index_;
	};

	struct TAllUserBan
	{
		tBYTE	group_id_;
	};

	struct TUseNProtectAuth
	{
		tBYTE use_flag_;
	};

	struct TAutouserBan
	{
		tINT	user_index_;
	};

	struct TChangeServerSetting
	{
		tINT group_id_;
		tFLOAT kill_mob_exp_rate_;
	};

	struct TGetServerSetting
	{
		tINT group_id_;
	};

	struct TGetStatusValue
	{
		tINT group_id_;
	};

	struct TChangeStatusValue
	{
		tINT group_id_;
		tINT value_[SERVER_STATUS_VALUE_MAX];
	};

	struct TUpdateEventList
	{
	};

	struct TWebAwakeAccount
	{
		tINT user_index_;
	};

	struct TRealTimeEventUpdate
	{
		tINT group_id_;
	};

#pragma pack(pop)

	inline rnPacket* daemonRun(tCHAR* data)
	{
		rnPacket* packet = new rnPacket(sizeof(TDaemonRun));

		packet->setGroupType(PGROUP_MONITOR_REQUEST, PTYPE_MONITOR_DAEMON_RUN);
		TDaemonRun* run = reinterpret_cast<TDaemonRun*>(packet->data());
		STRNCPY( run->execCommand_, data, sizeof( run->execCommand_ ) );
		packet->setDataSizeWithoutHeader(sizeof(TDaemonRun));

		return packet;
	}

	inline rnPacket* daemonKill(tCHAR* data)
	{
		rnPacket* packet = new rnPacket(sizeof(TDaemonRun));

		packet->setGroupType(PGROUP_MONITOR_REQUEST, PTYPE_MONITOR_DAEMON_KILL);
		TDaemonRun* kill = reinterpret_cast<TDaemonRun*>(packet->data());
		STRNCPY( kill->execCommand_, data, sizeof( kill->execCommand_ ) );
		packet->setDataSizeWithoutHeader(sizeof(TDaemonRun));

		return packet;
	}

	inline rnPacket* userLock(tINT userID)
	{
		rnPacket* packet = new rnPacket(sizeof(TUserLock));

		packet->setGroupType(PGROUP_MONITOR_REQUEST, PTYPE_MONITOR_USER_LOCK);
		TUserLock* lock = reinterpret_cast<TUserLock*>(packet->data());
		lock->user_index_ = userID;
		packet->setDataSizeWithoutHeader(sizeof(TUserLock));

		return packet;
	}

	inline rnPacket* userDataSync(tINT userID)
	{
		rnPacket* packet = new rnPacket(sizeof(TUserDataSync));

		packet->setGroupType(PGROUP_MONITOR_REQUEST, PTYPE_MONITOR_USER_DATA_SYNC);
		TUserDataSync* sync = reinterpret_cast<TUserDataSync*>(packet->data());
		sync->user_index_ = userID;
		packet->setDataSizeWithoutHeader(sizeof(TUserDataSync));

		return packet;
	}

	inline rnPacket* sendMailAlert( tINT user_index, tINT actor_index )
	{
		rnPacket* packet = new rnPacket(sizeof(TSendMailAlert));

		packet->setGroupType(PGROUP_MONITOR_REQUEST, PTYPE_MONITOR_SEND_MAIL_ALERT);
		TSendMailAlert* alert = reinterpret_cast<TSendMailAlert*>(packet->data());
		alert->user_index_	= user_index;
		alert->actor_index_	= actor_index;
		packet->setDataSizeWithoutHeader(sizeof(TSendMailAlert));

		return packet;
	}

	inline rnPacket* allUserBan( tBYTE group_id )
	{
		rnPacket* packet = new rnPacket(sizeof(TAllUserBan));

		packet->setGroupType(PGROUP_MONITOR_REQUEST, PTYPE_MONITOR_ALL_USER_BAN);
		TAllUserBan* data = reinterpret_cast<TAllUserBan*>(packet->data());
		data->group_id_ = group_id;
		packet->setDataSizeWithoutHeader(sizeof(TAllUserBan));

		return packet;
	}

	inline rnPacket* useNProtectAuth( tBYTE use_flag )
	{
		rnPacket* packet = new rnPacket(sizeof(TUseNProtectAuth));

		packet->setGroupType( PGROUP_MONITOR_REQUEST, PTYPE_MONITOR_USE_NPROTECT_AUTH );
		TUseNProtectAuth* data = reinterpret_cast<TUseNProtectAuth*>(packet->data());
		data->use_flag_ = use_flag;
		packet->setDataSizeWithoutHeader(sizeof(TUseNProtectAuth));
		return packet;
	}

	inline rnPacket* autouserBan(tINT user_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TAutouserBan));

		packet->setGroupType(PGROUP_MONITOR_REQUEST, PTYPE_MONITOR_AUTOUSER_BAN);
		TAutouserBan* ban = reinterpret_cast<TAutouserBan*>(packet->data());
		ban->user_index_ = user_id;
		packet->setDataSizeWithoutHeader(sizeof(TAutouserBan));

		return packet;
	}

	inline rnPacket* getServerSetting( tINT group_id )
	{
		rnPacket* packet = new rnPacket(sizeof(TChangeServerSetting));

		packet->setGroupType(PGROUP_MONITOR_REQUEST, PTYPE_MONITOR_GET_SERVER_SETTING);
		TChangeServerSetting* ban = reinterpret_cast<TChangeServerSetting*>(packet->data());
		ban->group_id_ = group_id;
		packet->setDataSizeWithoutHeader(sizeof(TChangeServerSetting));

		return packet;
	}

	inline rnPacket* changeStatusValue( tINT group_id, tINT* value )
	{
		rnPacket* packet = new rnPacket(sizeof(TChangeStatusValue));

		packet->setGroupType(PGROUP_MONITOR_REQUEST, PTYPE_MONITOR_CHANGE_STATUS_VALUE);
		TChangeStatusValue* data = reinterpret_cast<TChangeStatusValue*>(packet->data());
		data->group_id_ = group_id;
		memcpy( data->value_, value, sizeof(data->value_) );
		packet->setDataSizeWithoutHeader(sizeof(TChangeStatusValue));

		return packet;
	}

	inline rnPacket* realtimeEventUpdate( tINT group_id )
	{
		rnPacket* packet = new rnPacket(sizeof(TRealTimeEventUpdate));

		packet->setGroupType( PGROUP_MONITOR_REQUEST, PTYPE_MONITOR_REALTIME_EVENT_UPDATE );
		TRealTimeEventUpdate* data = reinterpret_cast<TRealTimeEventUpdate*>(packet->data());
		data->group_id_ = group_id;
		packet->setDataSizeWithoutHeader(sizeof(TRealTimeEventUpdate));

		return packet;
	}
}
