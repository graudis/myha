#pragma once

namespace MonitorResponse
{
#pragma pack(push, 1)

	struct TGroupUsers
	{
		tINT	group_id_;
		tINT	total_login_users_;
		tINT	crs_login_users_;
		tINT	nhn_login_users_;
		tINT	total_map_users_;
		tINT	crs_map_users_;
		tINT	nhn_map_users_;
	};

	struct TCurrentConnectUsers
	{
		tINT		group_count_;
		TGroupUsers	group_users_[0];
	};

	struct TUserDataSync
	{
		tINT	actor_id_;
	};

	struct TDaemonStartAlert
	{
		tINT		group_id_;
		tBYTE		sort_;
		tUSINT		id_;
		tUSINT		channelid_;
		tINT		process_id_;
		tCHAR		service_ip_[IP_STRING_MAX];
	};

	struct TDaemonEndAlert
	{
		tINT		process_id_;
		tCHAR		service_ip_[IP_STRING_MAX];
	};

	struct TChannelDaemonRun
	{
		tINT		group_id_;
		tINT		map_id_;
		tINT		channel_id_;
	};

	struct TLoginConnectCountInfo
	{
		tINT		group_id_;
		tINT		limit_count_;
	};

	struct TMapConnectCountInfo
	{
		tINT		group_id_;
		tINT		map_id_;
		tINT		limit_count_;
	};

	struct TGetServerSetting
	{
		tINT group_id_;
		tFLOAT kill_mob_exp_rate_;
	};

	struct TChangeStatusValue
	{
		tINT group_id_;
		tINT value_[SERVER_STATUS_VALUE_MAX];
	};

	struct TGetStatusValue
	{
		tINT group_id_;
		tINT value_[SERVER_STATUS_VALUE_MAX];
	};

#pragma pack(pop)

	// function
	inline rnPacket* userDataSync(tINT actor_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TUserDataSync));

		packet->setGroupType(PGROUP_MONITOR_RESPONSE, PTYPE_MONITOR_USER_DATA_SYNC);
		TUserDataSync* data = reinterpret_cast<TUserDataSync*>(packet->data());
		data->actor_id_ = actor_id;
		packet->setDataSizeWithoutHeader(sizeof(TUserDataSync));

		return packet;
	}

	inline rnPacket* daemonStartAlert(tINT group_id, tBYTE sort, tUSINT id, tUSINT channelid, tINT process_id, const tCHAR* service_ip)
	{
		rnPacket* packet = new rnPacket(sizeof(TDaemonStartAlert));

		packet->setGroupType(PGROUP_MONITOR_RESPONSE, PTYPE_MONITOR_DAEMON_START_ALERT);
		TDaemonStartAlert* data = reinterpret_cast<TDaemonStartAlert*>(packet->data());
		data->group_id_			= group_id;
		data->sort_				= sort;
		data->id_				= id;
		data->channelid_			= channelid;
		data->process_id_		= process_id;
		STRNCPY(data->service_ip_, service_ip, IP_STRING_MAX);
		packet->setDataSizeWithoutHeader(sizeof(TDaemonStartAlert));

		return packet;
	}

	inline rnPacket* daemonEndAlert(tINT process_id, const tCHAR* service_ip)
	{
		rnPacket* packet = new rnPacket(sizeof(TDaemonEndAlert));

		packet->setGroupType(PGROUP_MONITOR_RESPONSE, PTYPE_MONITOR_DAEMON_END_ALERT);
		TDaemonEndAlert* data = reinterpret_cast<TDaemonEndAlert*>(packet->data());
		data->process_id_	= process_id;
		STRNCPY(data->service_ip_, service_ip, IP_STRING_MAX);
		packet->setDataSizeWithoutHeader(sizeof(TDaemonEndAlert));

		return packet;
	}

	inline rnPacket* channelDaemonRun(tINT group_id, tINT map_id, tINT channel_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TChannelDaemonRun));

		packet->setGroupType(PGROUP_MONITOR_RESPONSE, PTYPE_MONITOR_CHANNEL_DAEMON_RUN);
		TChannelDaemonRun* data = reinterpret_cast<TChannelDaemonRun*>(packet->data());
		data->group_id_		= group_id;
		data->map_id_		= map_id;
		data->channel_id_	= channel_id;
		packet->setDataSizeWithoutHeader(sizeof(TChannelDaemonRun));

		return packet;
	}

	inline rnPacket* loginConnectCountInfo( tINT group_id, tINT limit_count )
	{
		rnPacket* packet = new rnPacket(sizeof(TLoginConnectCountInfo));

		packet->setGroupType(PGROUP_MONITOR_RESPONSE, PTYPE_MONITOR_LOGIN_CONNECT_COUNT_INFO);
		TLoginConnectCountInfo* data = reinterpret_cast<TLoginConnectCountInfo*>(packet->data());
		data->group_id_		= group_id;
		data->limit_count_	= limit_count;
		packet->setDataSizeWithoutHeader(sizeof(TLoginConnectCountInfo));

		return packet;
	}

	inline rnPacket* mapConnectCountInfo( tINT group_id, tINT map_id, tINT limit_count )
	{
		rnPacket* packet = new rnPacket(sizeof(TMapConnectCountInfo));

		packet->setGroupType(PGROUP_MONITOR_RESPONSE, PTYPE_MONITOR_MAP_CONNECT_COUNT_INFO);
		TMapConnectCountInfo* data = reinterpret_cast<TMapConnectCountInfo*>(packet->data());
		data->group_id_		= group_id;
		data->map_id_		= map_id;
		data->limit_count_	= limit_count;
		packet->setDataSizeWithoutHeader(sizeof(TMapConnectCountInfo));

		return packet;
	}

	inline rnPacket* getServerSetting( tINT group_id, tFLOAT kill_mob_exp_rate )
	{
		rnPacket* packet = new rnPacket(sizeof(TGetServerSetting));

		packet->setGroupType(PGROUP_MONITOR_RESPONSE, PTYPE_MONITOR_GET_SERVER_SETTING);
		TGetServerSetting* data = reinterpret_cast<TGetServerSetting*>(packet->data());
		data->group_id_ = group_id;
		data->kill_mob_exp_rate_ = kill_mob_exp_rate;
		packet->setDataSizeWithoutHeader(sizeof(TGetServerSetting));

		return packet;
	}

	inline rnPacket* changeStatusValue( tINT group_id, tINT* value )
	{
		rnPacket* packet = new rnPacket(sizeof(TChangeStatusValue));

		packet->setGroupType(PGROUP_MONITOR_RESPONSE, PTYPE_MONITOR_CHANGE_STATUS_VALUE);
		TChangeStatusValue* data = reinterpret_cast<TChangeStatusValue*>(packet->data());
		data->group_id_ = group_id;
		memcpy( data->value_, value, sizeof(data->value_) );
		packet->setDataSizeWithoutHeader(sizeof(TChangeStatusValue));

		return packet;
	}

	inline rnPacket* getStatusValue( tINT group_id, tINT* value )
	{
		rnPacket* packet = new rnPacket(sizeof(TGetStatusValue));

		packet->setGroupType(PGROUP_MONITOR_RESPONSE, PTYPE_MONITOR_GET_STATUS_VALUE);
		TGetStatusValue* data = reinterpret_cast<TGetStatusValue*>(packet->data());
		data->group_id_ = group_id;
		memcpy( data->value_, value, sizeof(data->value_) );
		packet->setDataSizeWithoutHeader(sizeof(TGetStatusValue));

		return packet;
	}
}
