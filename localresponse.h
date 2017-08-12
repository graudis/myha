#pragma once

#include "localtype.h"
#include "define_instance.h"

enum InMapError
{
	INMAP_NO_ERROR,										// 에러 없음
	INMAP_ERROR_NOT_INIT_CONTAINER,						// 컨테이너가 초기화 되지 않았음
	INMAP_ERROR_ALREADY_ENTER_MAP,						// 이미 맵에 들어가 있음
	INMAP_ERROR_MISSIONMAP_QUEUE_FAILED,				// 미션맵 큐에 없음
	INMAP_ERROR_MISSIONMAP_ADD_FAILED,					// 미션맵 입장 실패
	INMAP_ERROR_TOURNAMENT_WAITING_MAP_ADD_FAILED,		// 토너먼트 대기맵 입장 실패
	INMAP_ERROR_TOURNAMENT_BATTLE_MAP_QUEUE_FAILED,		// 토너먼트 전투맵 큐에 없음
	INMAP_ERROR_TOURNAMENT_BATTLE_MAP_ADD_FAILED,		// 토너먼트 전투맵 입장 실패
	INMAP_ERROR_QUEUE_ERROR,							// 큐 플래그 에러
	INMAP_ERROR_NOT_ENTER_MAP,							// 맵에 들어가 있지 않음
	INMAP_ERROR_CONNECT_FAILED,							// 프록시로 접속 실패
};

namespace CLocalResponse
{
#pragma pack(push, 1)

	struct TCheckMovableMap
	{
		tINT	actor_id_;
		tUSINT	map_id_;
		tUSINT	channel_id_;
		tFLOAT	x_;
		tFLOAT	z_;
		tINT	move_state_;
		tINT	result_;
		tINT	use_portal_id_;
		tINT	difficulty_;
		tINT	instance_id_;
		tBOOL	party_bind_;
	};

	struct TInMap
	{
		tBYTE reason_;
		tINT user_index_;
		tINT actor_id_;
	};

	struct TChannelUserCount
	{
		tINT	crs_user_count_;
		tINT	nhn_user_count_;
	};

	struct TServiceLock
	{
		tBOOL	is_lock_;
	};

	struct TChatBan
	{
		tINT	actor_index_;
		tINT	chat_ban_limit_time_;
	};

	// gc -> center -> gc
	struct TJoinMissionMapRes
	{
		tINT from_map_id_;
		tINT from_channel_id_;
		tINT to_map_id_;
		tINT to_channel_id_;
		tINT state_;
		tINT actor_id_;
	};

	struct TPcbangFlag
	{
		tINT actor_id_;
		tBOOL pcbang_flag_;
	};

	struct TServiceGroupLock
	{
		tBYTE	group_id_;
		tBOOL	is_lock_;
	};

	struct TQueryBoundInstance
	{
		tBYTE reason_;
		tINT map_id_;
		tINT user_id_;
		tFLOAT x_;
		tFLOAT z_;
		tINT use_portal_id_;
		tINT difficulty_;
		tINT channel_id_;
		tINT instance_id_;
		tBOOL party_bind_;
		tINT move_state_;
	};

	struct TBillingServiceLock
	{
		tBOOL is_lock_;
	};

	struct TChangeServerSetting
	{
		tFLOAT kill_mob_exp_rate_;
	};

	struct TResetInstance
	{
		tBYTE reason_;
		tINT user_id_;
		tINT map_id_;
		tINT difficulty_;
		tBYTE use_item_;
	};

	struct TResetInstanceList
	{
		tINT user_id_;
		tINT reset_count_;

		struct TResetInstanceInfo
		{
			tINT map_id_;
			tINT difficulty_;
		} reset_info_[0];
	};

	struct TInstanceCooltimeList
	{
		tINT user_id_;
		tINT instance_count_;

		TInstanceInfo instance_info_[0];
	};

	struct TAddInstanceCooltime
	{
		tINT user_id_;
		TInstanceInfo instance_info_;
	};

	struct TUpdateInstanceUnloadTime
	{
		tINT user_id_;
		TInstanceInfo instance_info_;
	};

	struct TCenterInMap
	{
		tBYTE reason_;
		tINT user_id_;
	};

	struct TCenterOutMap
	{
		tBYTE reason_;
		tINT user_id_;
	};

	struct TRaidMobRegenStatus
	{
		tINT actor_id_;
		tINT count_;

		struct RegenData
		{
			tINT map_id_;
			tINT channel_id_;
			tINT mob_id_;
			tINT regen_time_;
		} data_[0];
	};

#pragma pack(pop)

	inline rnPacket* checkMovableMap(TCheckMovableMap& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TCheckMovableMap));
		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_CHECK_MOVABLE_MAP);
		packet->addVALUE<TCheckMovableMap>(data);
		return packet;
	}

	inline rnPacket* inMap(tBYTE reason, tINT user_index, tINT actor_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TInMap));
		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_IN_MAP);

		TInMap* data = reinterpret_cast<TInMap*>(packet->data());
		data->reason_ = reason;
		data->user_index_ = user_index;
		data->actor_id_ = actor_id;
		packet->setDataSizeWithoutHeader(sizeof(TInMap));

		return packet;
	}

	inline rnPacket* channelUserCount(tINT crs_user_count, tINT nhn_user_count)
	{
		rnPacket* packet = new rnPacket(sizeof(TChannelUserCount));
		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_CHANNEL_USER_COUNT);
		TChannelUserCount* data = reinterpret_cast<TChannelUserCount*>(packet->data());
		data->crs_user_count_ = crs_user_count;
		data->nhn_user_count_ = nhn_user_count;
		packet->setDataSizeWithoutHeader(sizeof(TChannelUserCount));
		return packet;
	}

	inline rnPacket* serverInfo(tINT type, tINT id, tINT channel)
	{
		rnPacket* packet = new rnPacket(sizeof(TServerInfo));

		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_SERVER_INFO);
		TServerInfo* data = reinterpret_cast<TServerInfo*>(packet->data());
		data->type_ = type;
		data->id_ = id;
		data->channel_ = channel;
		packet->setDataSizeWithoutHeader(sizeof(TServerInfo));

		return packet;
	}

	inline rnPacket* listenInfo(tINT for_type, const tCHAR* ip, tINT port)
	{
		rnPacket* packet = new rnPacket(sizeof(TListenInfo));

		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_CONNECTION_INFO);
		TListenInfo* data = reinterpret_cast<TListenInfo*>(packet->data());
		data->for_type_ = for_type;
		STRNCPY(data->ip_, ip, sizeof(data->ip_));
		data->port_ = port;
		packet->setDataSizeWithoutHeader(sizeof(TListenInfo));

		return packet;
	}

	inline rnPacket* serviceLock(tBOOL is_lock)
	{
		rnPacket* packet = new rnPacket(sizeof(TServiceLock));

		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_LOGIN_LOCK);
		TServiceLock* data = reinterpret_cast<TServiceLock*>(packet->data());
		data->is_lock_ = is_lock;
		packet->setDataSizeWithoutHeader(sizeof(TServiceLock));

		return packet;
	}

	inline rnPacket* chatBan(TChatBan& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatBan));

		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_CHAT_BAN);
		packet->addVALUE<TChatBan>(data);

		return packet;
	}

	inline rnPacket* joinMissionMapRes(tINT from_map_id, tINT from_channel_id, tINT to_map_id, tINT to_channel_id, tINT state, tINT actor_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TJoinMissionMapRes));
		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_JOIN_MISSION_MAP_RES);

		TJoinMissionMapRes* data = reinterpret_cast<TJoinMissionMapRes*>(packet->data());
		data->from_map_id_ = from_map_id;
		data->from_channel_id_ = from_channel_id;
		data->to_map_id_ = to_map_id;
		data->to_channel_id_ = to_channel_id;
		data->state_ = state;
		data->actor_id_ = actor_id;
		packet->setDataSizeWithoutHeader(sizeof(TJoinMissionMapRes));
		return packet;
	}

	inline rnPacket* pcbangFlag(tINT actor_id, tBOOL pcbang_flag)
	{
		rnPacket* packet = new rnPacket(sizeof(TPcbangFlag));
		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_PCBANG_FLAG);

		TPcbangFlag* data = reinterpret_cast<TPcbangFlag*>(packet->data());
		data->actor_id_ = actor_id;
		data->pcbang_flag_ = pcbang_flag;
		packet->setDataSizeWithoutHeader(sizeof(TPcbangFlag));
		return packet;
	}

	inline rnPacket* serviceGroupLock(tINT group_id, tBOOL is_lock)
	{
		rnPacket* packet = new rnPacket(sizeof(TServiceGroupLock));

		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_SERVICE_GROUP_LOCK);
		TServiceGroupLock* data = reinterpret_cast<TServiceGroupLock*>(packet->data());
		data->group_id_ = group_id;
		data->is_lock_ = is_lock;
		packet->setDataSizeWithoutHeader(sizeof(TServiceGroupLock));

		return packet;
	}

	inline rnPacket* queryBoundInstance(TQueryBoundInstance& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TQueryBoundInstance));
		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_QUERY_BOUND_INSTANCE);
		packet->addVALUE<TQueryBoundInstance>(data);
		return packet;
	}

	inline rnPacket* billingServiceLock(tBOOL is_lock)
	{
		rnPacket* packet = new rnPacket(sizeof(TBillingServiceLock));
		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_BILLING_SERVICE_LOCK);

		TBillingServiceLock* data = reinterpret_cast<TBillingServiceLock*>(packet->data());
		data->is_lock_ = is_lock;
		packet->setDataSizeWithoutHeader(sizeof(TBillingServiceLock));
		return packet;
	}

	inline rnPacket* changeServerSetting(tFLOAT kill_mob_exp_rate)
	{
		rnPacket* packet = new rnPacket(sizeof(TChangeServerSetting));
		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_CHANGE_SERVER_SETTING);

		TChangeServerSetting* data = reinterpret_cast<TChangeServerSetting*>(packet->data());
		data->kill_mob_exp_rate_ = kill_mob_exp_rate;
		packet->setDataSizeWithoutHeader(sizeof(TChangeServerSetting));
		return packet;
	}

	inline rnPacket* resetInstance(tBYTE reason, tINT user_id, tINT map_id, tINT difficulty, tBYTE use_item)
	{
		rnPacket* packet = new rnPacket(sizeof(TResetInstance));

		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_RESET_INSTANCE);
		TResetInstance* data = reinterpret_cast<TResetInstance*>(packet->data());
		data->reason_ = reason;
		data->user_id_ = user_id;
		data->map_id_ = map_id;
		data->difficulty_ = difficulty;
		data->use_item_ = use_item;
		packet->setDataSizeWithoutHeader(sizeof(TResetInstance));
		return packet;
	}

	inline rnPacket* centerInMap(tBYTE reason, tINT user_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TCenterInMap));

		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_CENTER_INMAP);
		TCenterInMap* data = reinterpret_cast<TCenterInMap*>(packet->data());
		data->reason_ = reason;
		data->user_id_ = user_id;
		packet->setDataSizeWithoutHeader(sizeof(TCenterInMap));
		return packet;
	}

	inline rnPacket* centerOutMap(tBYTE reason, tINT user_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TCenterOutMap));

		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_CENTER_OUTMAP);
		TCenterOutMap* data = reinterpret_cast<TCenterOutMap*>(packet->data());
		data->reason_ = reason;
		data->user_id_ = user_id;
		packet->setDataSizeWithoutHeader(sizeof(TCenterOutMap));
		return packet;
	}

}
