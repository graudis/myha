#pragma once

#include "localtype.h"

namespace CLocalAnnounce
{
#pragma pack(push, 1)

	struct TDeleteActor
	{
		tCHAR	actor_name_[STATUS_NAME_MAX];	// 삭제한 캐릭명
		tINT	actor_id_;						// 삭제한 캐릭번호
	};

	struct TMissionMapAnnounce
	{
		tINT	map_id_;
		tINT	type_id_;
		tINT	mission_id_;
		tINT	flag_;
		tINT	min_level_;
		tINT	max_level_;
		tINT	status_;
		tINT	max_players_;
	};

	//XX 잭팟 패킷 (announce)
	struct TJackpotCoin
	{
		jackpot_coin_t	coin_;
	};

	struct TJackpotResult
	{
		tINT	actor_id_;
		tCHAR	actor_name_[STATUS_NAME_MAX];
		tBYTE	actor_level_;
		tBYTE	grade_;
		tBYTE	reward_type_;
		tINT64	reward_id_;
	};

	struct TCurrentUserCount
	{
		tTIME	time;
	};

	struct TGuildInfoUpdate
	{
		tINT	actor_id_;
		tINT	guild_id_;
	};

	struct TProduceSuccessAnnounce
	{
		tINT	actor_id_;
		tINT	case_index_;
	};

	struct TDaemonProcessInfo
	{
		tBYTE	sort_;
		tUSINT	id_;
		tUSINT	channelid_;
		tINT	process_id_;
	};

	struct TChannelMapInfo
	{
		tBOOL	is_channel_map_;
		tINT	max_user_count_;
	};

	struct TPacketVersion
	{
		tINT	packet_version_;
	};

	struct TBanName
	{
		tCHAR	ban_actor_name_[STATUS_NAME_MAX];
		tBYTE	reason_;
		tINT	action_actor_id_;			// 밴을 시킨 주체(0 - system)
		tUINT	unixtime_;
	};

	struct TLuckyRouletteResetTime
	{
	};

	struct TChangeWDay
	{
		tINT wday_;
	};


	struct TStopRoulette
	{
	};

	struct TEventFlagSesoTime
	{
		tINT		start_clock_;
		tINT		end_clock_;
		tBOOL		is_active_event_;
		tINT		event_persent_;
	};

	struct TDBStatus
	{
		tINT	is_master;	// 1 : slave, 0 : master
		tINT	state;		// 0 : OK,  1~ : NOT OK
	};

#pragma pack(pop)

	inline rnPacket* deleteActor(tINT actor_id, const tCHAR* actor_name)
	{
		rnPacket* packet = new rnPacket(sizeof(TDeleteActor));

		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_DELETE_ACTOR);
		TDeleteActor* delete_actor = reinterpret_cast<TDeleteActor*>(packet->data());
		delete_actor->actor_id_ = actor_id;
		STRNCPY(delete_actor->actor_name_, actor_name, sizeof(delete_actor->actor_name_));
		packet->setDataSizeWithoutHeader(sizeof(TDeleteActor));

		return packet;
	}

	inline rnPacket* missionMapAnnounce(tINT map_id, tINT type_id, tINT mission_id, tINT flag, tINT min_level, tINT max_level, tINT status, tINT max_players)
	{
		rnPacket* packet = new rnPacket(sizeof(TMissionMapAnnounce));

		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_MISSION_MAP_ANNOUNCE);
		TMissionMapAnnounce* data = reinterpret_cast<TMissionMapAnnounce*>(packet->data());
		data->map_id_ = map_id;
		data->type_id_ = type_id;
		data->mission_id_ = mission_id;
		data->flag_ = flag;
		data->min_level_ = min_level;
		data->max_level_ = max_level;
		data->status_ = status;
		data->max_players_ = max_players;
		packet->setDataSizeWithoutHeader(sizeof(TMissionMapAnnounce));

		return packet;
	}

	inline rnPacket* jackpotCoin(jackpot_coin_t coin)
	{
		rnPacket* packet = new rnPacket(sizeof(TJackpotCoin));

		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_JACKPOT_MONEY);
		TJackpotCoin* data = reinterpret_cast<TJackpotCoin*>(packet->data());
		data->coin_ = coin;
		packet->setDataSizeWithoutHeader(sizeof(TJackpotCoin));

		return packet;
	}

	inline rnPacket* jackpotResult(TJackpotResult* data)
	{
		rnPacket* packet = new rnPacket(sizeof(TJackpotResult));
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_JACKPOT_RESULT_BROADCASTING);
		packet->addVALUE((tBYTE*)data, sizeof(TJackpotResult));
		return packet;
	}

	inline rnPacket* currentConnectUser()
	{
		rnPacket* packet = new rnPacket(sizeof(TCurrentUserCount));

		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_CURRENT_CONNECT_USERS);
		TCurrentUserCount* data = reinterpret_cast<TCurrentUserCount*>(packet->data());
		data->time = time(0);
		packet->setDataSizeWithoutHeader(sizeof(TCurrentUserCount));

		return packet;
	}

	inline rnPacket* produceSuccessAnnounce(tINT actor_id, tINT case_index)
	{
		rnPacket* packet = new rnPacket(sizeof(TProduceSuccessAnnounce));

		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_PRODUCE_SUCCESS_ANNOUNCE);
		TProduceSuccessAnnounce* data = reinterpret_cast<TProduceSuccessAnnounce*>(packet->data());
		data->actor_id_ = actor_id;
		data->case_index_ = case_index;
		packet->setDataSizeWithoutHeader(sizeof(TProduceSuccessAnnounce));

		return packet;
	}

	inline rnPacket* daemonprocessinfo(tBYTE sort, tUSINT id, tUSINT channelid, tINT process_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TDaemonProcessInfo));

		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_PROCESS_INFO);
		TDaemonProcessInfo* data = reinterpret_cast<TDaemonProcessInfo*>(packet->data());
		data->sort_ = sort;
		data->id_ = id;
		data->channelid_ = channelid;
		data->process_id_ = process_id;
		packet->setDataSizeWithoutHeader(sizeof(TDaemonProcessInfo));

		return packet;
	}

	inline rnPacket* channelMapInfo(tBOOL is_channel_map, tINT max_user_count)
	{
		rnPacket* packet = new rnPacket(sizeof(TChannelMapInfo));

		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_CHANNEL_MAP_INFO);
		TChannelMapInfo* data = reinterpret_cast<TChannelMapInfo*>(packet->data());
		data->is_channel_map_ = is_channel_map;
		data->max_user_count_ = max_user_count;
		packet->setDataSizeWithoutHeader(sizeof(TChannelMapInfo));

		return packet;
	}

	inline rnPacket* connectionInfo(tINT for_type, tINT id, tINT channel, tCHAR* ip, tINT port)
	{
		rnPacket* packet = new rnPacket(sizeof(TConnectionInfo));

		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_CONNECTION_INFO);
		TConnectionInfo* data = reinterpret_cast<TConnectionInfo*>(packet->data());
		data->type_ = for_type;
		data->id_ = id;
		data->channelid_ = channel;
		STRNCPY(data->ip_, ip, sizeof(data->ip_));
		data->port_ = port;
		packet->setDataSizeWithoutHeader(sizeof(TConnectionInfo));

		return packet;
	}

	inline rnPacket* packetVersion(tINT packet_version)
	{
		rnPacket* packet = new rnPacket(sizeof(TPacketVersion));

		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_PACKET_VERSION);

		TPacketVersion* data = reinterpret_cast<TPacketVersion*>(packet->data());
		data->packet_version_ = packet_version;

		packet->setDataSizeWithoutHeader(sizeof(TPacketVersion));

		return packet;
	}

	inline rnPacket* banName(TBanName& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TBanName));
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_BAN_NAME);
		packet->addVALUE(data);
		return packet;
	}

	inline rnPacket* ping()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_PING);
		return packet;
	}

	inline rnPacket* updateDailyQuest()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_UPDATE_DAILY_QUEST);
		return packet;
	}

	inline rnPacket* updateDailyPcbangEvnet()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_UPDATE_DAILY_PCBANG_EVENT);
		return packet;
	}

	inline rnPacket* updateDailyTowerCount()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_UPDATE_DAILY_TOWER_COUNT);
		return packet;
	}

	inline rnPacket* luckyRouletteResetTime()
	{
		rnPacket* packet = new rnPacket(sizeof(TLuckyRouletteResetTime));
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_LUCKY_ROULETTE_RESET_TIME);
		return packet;
	}
	inline rnPacket* changeWDay(tINT wday)
	{
		rnPacket* packet = new rnPacket(sizeof(TChangeWDay));
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_CHANGE_DAY);

		TChangeWDay* data = reinterpret_cast<TChangeWDay*>(packet->data());

		data->wday_ = wday;

		packet->setDataSizeWithoutHeader(sizeof(TChangeWDay));
		return packet;
	}

	inline rnPacket* stopRoulette()
	{
		rnPacket* packet = new rnPacket(sizeof(TStopRoulette));
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_STOP_ROULETTE);

		packet->setDataSizeWithoutHeader(sizeof(TStopRoulette));
		return packet;
	}

	inline rnPacket* eventFlagSesoTime(tINT start_clock, tINT end_clock, tBOOL is_active_event, tINT event_persent)
	{
		rnPacket* packet = new rnPacket(sizeof(TEventFlagSesoTime));
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_EVENT_FLAG_SESO_TIME);

		TEventFlagSesoTime* data = reinterpret_cast<TEventFlagSesoTime*>(packet->data());
		data->start_clock_ = start_clock;
		data->end_clock_ = end_clock;
		data->is_active_event_ = is_active_event;
		data->event_persent_ = event_persent;
		packet->setDataSizeWithoutHeader(sizeof(TEventFlagSesoTime));
		return packet;
	}

	inline rnPacket* updateDailyBrokerQuest()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_UPDATE_DAILY_BROKER_QUEST);
		return packet;
	}
	inline rnPacket* resetOneDayQuest()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_RESET_COMEPLETED_ONEDAY_QUEST_INFO);
		return packet;
	}

	inline rnPacket* resetTodayMission()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_RESET_TODAY_MISSION);
		return packet;
	}

	inline rnPacket* updateInsResetCount()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_UPDATE_INS_RESET_COUNT);
		return packet;
	}

	inline rnPacket* serverInit()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_SERVER_INIT);
		packet->setDataSizeWithoutHeader(0);
		return packet;
	}

	inline rnPacket* DBStatus(TDBStatus& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TDBStatus));

		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_DB_STATUS);
		packet->addVALUE<TDBStatus>(data);

		return packet;
	}
}
