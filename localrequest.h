#pragma once

#include "localtype.h"
#include "define_mail.h"

namespace CLocalRequest
{
#pragma pack(push, 1)

	struct TLoginProxyAdd
	{
		tINT id_;
		tCHAR ip_[IP_STRING_MAX];
	};

	struct TLoginSession
	{
		tINT			count_;
		TLoginCount		list_[0];
	};

	struct TServiceLock
	{
		tBOOL			is_lock_;
	};

	struct TCheckMovableMap
	{
		tINT			actor_id_;
		tUSINT			map_id_;
		tUSINT			channel_id_;
		tFLOAT			x_;
		tFLOAT			z_;
		tINT			move_state_;
		tINT			use_portal_id_;
		tINT			difficulty_;
		tINT			instance_id_;
		tBOOL			party_bind_;
	};

	struct TInMap
	{
		tINT			actor_id_;
		tINT			ip_;
		tINT			port_;
		tINT			proxy_id_;
		TShare			share_;
		TStatus			status_;
		TUserDB			user_db_;
		tBYTE			move_state_;
		tUINT			inmap_time_;
		tINT			site_code_;
		tBOOL			self_certification_;
		tUINT			combine_keep_use_end_time_;

		TMapToMapUserData user_data_;
	};

	struct TMoveMap
	{
		tINT			actor_id_;
		tUSINT			map_id_;
		tUSINT			channel_id_;
		tFLOAT			x_;
		tFLOAT			z_;
		tBYTE			move_state_;

		TMapToMapUserData user_data_;
	};

	struct TOutMap
	{
		tINT			actor_id_;
	};

	struct TPUserCount
	{
		tINT			crs_user_count_;
		tINT			nhn_user_count_;
	};

	struct TLoginLimit
	{
		tINT			total_limit_;
		tINT			proxy_count_max_;
	};

	struct TLoginProxyDelete
	{
		tINT			id_;
	};

	struct TCompletedQuest
	{
		tINT	user_idnex_;
		tINT	actor_id_;
		tSINT	count_;
		tSINT	quest_id_[0];
	};

	struct TRealtimeEventUpdate
	{
		tINT map_count_;
		TRealTimePercent event_info_[0];
	};

	struct TLoginChatNew
	{
		tCHAR	ip_[IP_STRING_MAX];
		tINT	port_;
	};

	struct TLoginCahtDel
	{
		tINT	group_id_;
	};

	struct TActorOption
	{
		tINT		actor_id_;
		tUINT		option_;
	};

	struct TUpdateLoginConnectCount
	{
		tINT	limit_count_;
	};

	struct TUpdateMapConnectCount
	{
		tINT	map_id_;
		tINT	limit_count_;
	};

	struct TAdminForceLogout
	{
		tINT		group_id_;
		tINT		user_index_;
		tINT		actor_index_;
	};

	struct TForceOutMap
	{
		tINT		user_index_;
		tINT		actor_index_;
	};

	struct TChatBan
	{
		tCHAR		actor_name_[STATUS_NAME_MAX];
		tINT		chat_ban_limit_time_;
	};

	struct TServiceExpireTime
	{
		tINT		expire_time_;
	};

	struct TOpenMissionMap
	{
		tINT map_id_;
		tINT type_;
		tINT min_level_;
		tINT max_level_;
		tINT max_players_;
	};

	struct TCloseMissionMap
	{
		tINT map_id_;
	};

	struct TJoinMissionMapReq
	{
		tINT from_map_id_;
		tINT from_channel_id_;
		tINT to_map_id_;
		tINT to_channel_id_;
		tINT actor_id_;
		tINT actor_level_;
		coin_t actor_coin_;
		fame_t actor_fame_;
	};

	struct TRemoveQueueMissionMap
	{
		tINT actor_id_;
	};

	struct TPcbangFlag
	{
		tINT actor_id_;
		tINT pcbang_flag_;
	};

	struct TServiceGroupLock
	{
		tBYTE			group_id_;
		tBOOL			is_lock_;
	};

	struct TCreateInstance
	{
		tINT map_id_;
		tINT channel_id_;
		tINT instance_id_;
		tINT difficulty_;
	};

	struct TDestroyInstance
	{
		tINT map_id_;
		tINT channel_id_;
		tINT instance_id_;
	};

	struct TBindInstance
	{
		tINT map_id_;
		tINT channel_id_;
		tINT instance_id_;
		tINT difficulty_;
		tINT user_id_;
		tINT party_id_;
	};

	struct TUnBindUserInstance
	{
		tINT map_id_;
		tINT channel_id_;
		tINT difficulty_;
		tINT instance_id_;
		tINT user_id_;
	};

	struct TUnBindPartyInstance
	{
		tINT map_id_;
		tINT channel_id_;
		tINT difficulty_;
		tINT instance_id_;
		tINT party_id_;
	};

	struct TQueryBoundInstance
	{
		tINT map_id_;
		tINT user_id_;
		tINT party_id_;
		tFLOAT x_;
		tFLOAT z_;
		tINT use_portal_id_;
		tINT difficulty_;
		tINT move_state_;
	};

	struct TBillingServiceLock
	{
		tBOOL is_lock_;
	};

	struct TAutouserBan
	{
		tINT actor_id_;
	};

	struct TAutoUserQuestionFastPacket
	{
		tINT actor_id_;
	};

	struct TChangeServerSetting
	{
		tFLOAT kill_mob_exp_rate_;
	};

	struct TAddPacketLog
	{
		tINT user_index_;
	};

	struct TRemovePacketLog
	{
		tINT user_index_;
	};

	struct TKickLoginServerUser
	{
		tINT login_type_;
		tCHAR user_id_[USER_ID_MAX];
	};

	struct TKickProxyServerUser
	{
		tINT user_index_;
	};

	struct TPostEvent
	{
		tCHAR			sender_name_[STATUS_NAME_MAX];
		tCHAR			title_[MAIL_TITLE_STRING_MAX];
		tCHAR			content_[MAIL_CONTENT_STRING_MAX];
		tINT			coin_;
		TOwnItem		item_;
		tINT			valid_time_;
		tBYTE			mail_type_;
		tINT			min_level_;
		tINT			max_level_;
	};

	struct TRecallPostEvent
	{
	};

	struct TCompletedOneDayQuest
	{
		tINT	user_idnex_;
		tINT	actor_id_;
		tSINT	count_;
		tSINT	quest_id_[0];
	};

	struct TResetInstance
	{
		tINT user_id_;
		tINT party_id_;
		tINT map_id_;
		tINT difficulty_;
		tINT remain_reset_count_;
		tINT inven_index_;
	};

	struct TResetInstanceAll
	{
		tINT user_id_;
	};

	struct TGetInstanceCooltime
	{
		tINT user_id_;
	};

	struct TResetInstanceCooltimeAll
	{
		tINT user_id_;
	};

	struct TStartDestroyInstance
	{
		tINT map_id_;
		tINT channel_id_;
		tINT instance_id_;
		tINT unload_time_;
	};

	struct TCancelDestroyInstance
	{
		tINT map_id_;
		tINT channel_id_;
		tINT instance_id_;
	};

	struct TCenterInMap
	{
		tINT user_id_;
	};

	struct TCenterOutMap
	{
		tINT user_id_;
	};


	struct TChangeActorName
	{
		tINT actor_id_;
		tCHAR new_name_[STATUS_NAME_MAX];
	};

	struct TRealtimeEventStatus
	{
		tBOOL status_;
	};

	struct TServerCommandUpdate
	{
	};

	struct TRaidMobRegenStatus
	{
		tINT actor_id_;
	};

	struct TSendScheduleMail
	{
		tINT server_group_;
		tINT schedule_no_;
	};

	struct TErrorETimedOut
	{
		tINT server_type_;
		tINT group_id_;
		tINT id_;
	};

#pragma pack(pop)

	inline rnPacket* end()
	{
		rnPacket* packet = new rnPacket(1);
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_END);
		return packet;
	}

	inline rnPacket* PUserCount(tINT crs_user_count, tINT nhn_user_count)
	{
		rnPacket* packet = new rnPacket(sizeof(TPUserCount));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_USER_COUNT);
		TPUserCount* send_data = reinterpret_cast<TPUserCount*>(packet->data());
		send_data->crs_user_count_ = crs_user_count;
		send_data->nhn_user_count_ = nhn_user_count;
		packet->setDataSizeWithoutHeader(sizeof(TPUserCount));

		return packet;
	}

	inline rnPacket* CLoginLimit(tINT total_limit, tINT proxy_count_max)
	{
		rnPacket* packet = new rnPacket(sizeof(TLoginLimit));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_LOGIN_LIMIT);
		TLoginLimit* send_data = reinterpret_cast<TLoginLimit*>(packet->data());
		send_data->total_limit_ = total_limit;
		send_data->proxy_count_max_ = proxy_count_max;
		packet->setDataSizeWithoutHeader(sizeof(TLoginLimit));

		return packet;
	}

	inline rnPacket* CLoginProxyNew(TLoginProxyAdd& proxy_server_info)
	{
		rnPacket* packet = new rnPacket(sizeof(TLoginProxyAdd));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_LOGIN_NEW);
		packet->addVALUE(proxy_server_info);
		return packet;
	}

	inline rnPacket* CLoginProxyDelete(tINT id)
	{
		rnPacket* packet = new rnPacket(sizeof(TLoginProxyDelete));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_LOGIN_DELETE);
		TLoginProxyDelete* send_data = reinterpret_cast<TLoginProxyDelete*>(packet->data());
		send_data->id_ = id;
		packet->setDataSizeWithoutHeader(sizeof(TLoginProxyDelete));

		return packet;
	}

	inline rnPacket* CLoginSession(TLoginCount& data)
	{
		rnPacket* packet = new rnPacket(sizeof(tINT) + sizeof(TLoginCount));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_LOGIN_SESSION);
		packet->addINT(1);
		packet->addVALUE<TLoginCount>(data);
		return packet;
	}

	inline rnPacket* serviceLock(tBOOL is_lock)
	{
		rnPacket* packet = new rnPacket(sizeof(TServiceLock));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_LOGIN_LOCK);

		TServiceLock data;

		data.is_lock_ = is_lock;

		packet->addVALUE<TServiceLock>(data);
		return packet;
	}

	inline rnPacket* checkMovableMap(tINT actor_id, tINT map_id, tINT channel_id, tFLOAT x, tFLOAT z, tINT move_state, tINT use_portal_id, tINT difficulty, tINT instance_id, tBOOL party_bind)
	{
		rnPacket* packet = new rnPacket(sizeof(TCheckMovableMap));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_CHECK_MOVABLE_MAP);
		TCheckMovableMap* move_map = reinterpret_cast<TCheckMovableMap*>(packet->data());
		move_map->actor_id_ = actor_id;
		move_map->map_id_ = map_id;
		move_map->channel_id_ = channel_id;
		move_map->x_ = x;
		move_map->z_ = z;
		move_map->move_state_ = move_state;
		move_map->difficulty_ = difficulty;
		move_map->use_portal_id_ = use_portal_id;
		move_map->instance_id_ = instance_id;
		move_map->party_bind_ = party_bind;
		packet->setDataSizeWithoutHeader(sizeof(TCheckMovableMap));

		return packet;
	}

	inline rnPacket* moveMap(TMoveMap& move_map)
	{
		rnPacket* packet = new rnPacket(sizeof(TMoveMap));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_MOVE_MAP);
		packet->addVALUE<TMoveMap>(move_map);

		return packet;
	}

	inline rnPacket* outMap(tINT actor_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TOutMap));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_OUT_MAP);
		TOutMap* data = reinterpret_cast<TOutMap*>(packet->data());
		data->actor_id_ = actor_id;
		packet->setDataSizeWithoutHeader(sizeof(TOutMap));
		return packet;
	}

	inline rnPacket* serverInfo(tINT type, tINT id, tINT channel)
	{
		rnPacket* packet = new rnPacket(sizeof(TServerInfo));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_SERVER_INFO);
		TServerInfo* data = reinterpret_cast<TServerInfo*>(packet->data());
		data->type_ = type;
		data->id_ = id;
		data->channel_ = channel;
		packet->setDataSizeWithoutHeader(sizeof(TServerInfo));

		return packet;
	}

	inline rnPacket* completedQuestInfo(tINT user_index, tINT actor_id, completed_quest_vec_t& completedqeust)
	{
		rnPacket* packet = new rnPacket(sizeof(TCompletedQuest) + (sizeof(tSINT) * completedqeust.size()));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_COMPLETED_QUEST_INFO);
		TCompletedQuest* data = reinterpret_cast<TCompletedQuest*>(packet->data());
		data->user_idnex_ = user_index;
		data->actor_id_ = actor_id;
		data->count_ = (tSINT)completedqeust.size();
		packet->setDataSizeWithoutHeader(sizeof(TCompletedQuest));

		//		completed_quest_vec_t::iterator begin = completedqeust.begin();
		//		completed_quest_vec_t::iterator end = completedqeust.end();
		for (tINT i = 0; i < (tINT)completedqeust.size(); i++)
		{
			packet->addSINT(completedqeust[i]);
		}

		return packet;
	}

	inline rnPacket* realtimeEventUpdate(tINT map_count, TRealTimePercent* event_info_)
	{
		rnPacket* packet = new rnPacket(sizeof(TRealTimePercent) * map_count);

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_REALTIME_EVENT_UPDATE);
		TRealtimeEventUpdate* data = reinterpret_cast<TRealtimeEventUpdate*>(packet->data());
		data->map_count_ = map_count;
		packet->setDataSizeWithoutHeader(sizeof(TRealtimeEventUpdate));

		for (tINT i = 0; i < map_count; i++)
		{
			packet->addVALUE<TRealTimePercent>(event_info_[i]);
		}

		return packet;
	}

	inline rnPacket* loginChatNew(TLoginChatNew& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TLoginChatNew));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_LOGIN_CHAT_NEW);
		packet->addVALUE<TLoginChatNew>(data);

		return packet;
	}

	inline rnPacket* loginChatDel(TLoginCahtDel& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TLoginCahtDel));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_LOGIN_CHAT_DEL);
		packet->addVALUE<TLoginCahtDel>(data);

		return packet;
	}

	inline rnPacket* actorOption(tINT actor_id, tUINT option)
	{
		rnPacket* packet = new rnPacket(sizeof(TActorOption));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_ACTOR_OPTION);
		TActorOption* data = reinterpret_cast<TActorOption*>(packet->data());
		data->actor_id_ = actor_id;
		data->option_ = option;
		packet->setDataSizeWithoutHeader(sizeof(TActorOption));

		return packet;
	}

	inline rnPacket* updateLoginConnectCount(tINT limit_count)
	{
		rnPacket* packet = new rnPacket(sizeof(TUpdateLoginConnectCount));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_UPDATE_LOGIN_CONNECT_COUNT);
		TUpdateLoginConnectCount* data = reinterpret_cast<TUpdateLoginConnectCount*>(packet->data());
		data->limit_count_ = limit_count;
		packet->setDataSizeWithoutHeader(sizeof(TUpdateLoginConnectCount));

		return packet;
	}

	inline rnPacket* updateMapConnectCount(tINT map_id, tINT limit_count)
	{
		rnPacket* packet = new rnPacket(sizeof(TUpdateMapConnectCount));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_UPDATE_MAP_CONNECT_COUNT);
		TUpdateMapConnectCount* data = reinterpret_cast<TUpdateMapConnectCount*>(packet->data());
		data->map_id_ = map_id;
		data->limit_count_ = limit_count;
		packet->setDataSizeWithoutHeader(sizeof(TUpdateMapConnectCount));

		return packet;
	}

	inline rnPacket* adminForceLogout(tINT group_id, tINT user_index, tINT actor_index)
	{
		rnPacket* packet = new rnPacket(sizeof(TAdminForceLogout));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_ADMIN_FORCE_LOGOUT);
		TAdminForceLogout* data = reinterpret_cast<TAdminForceLogout*>(packet->data());
		data->group_id_ = group_id;
		data->user_index_ = user_index;
		data->actor_index_ = actor_index;
		packet->setDataSizeWithoutHeader(sizeof(TAdminForceLogout));

		return packet;
	}

	inline rnPacket* forceOutMap(tINT user_index, tINT actor_index)
	{
		rnPacket* packet = new rnPacket(sizeof(TForceOutMap));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_FORCE_OUT_MAP);
		TForceOutMap* data = reinterpret_cast<TForceOutMap*>(packet->data());
		data->user_index_ = user_index;
		data->actor_index_ = actor_index;
		packet->setDataSizeWithoutHeader(sizeof(TForceOutMap));

		return packet;
	}

	inline rnPacket* chatBan(TChatBan& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatBan));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_CHAT_BAN);
		packet->addVALUE<TChatBan>(data);

		return packet;
	}

	inline rnPacket* serviceExpireTime(tINT expire_time)
	{
		rnPacket* packet = new rnPacket(sizeof(TServiceExpireTime));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_SERVICE_EXPIRE_TIME);

		TServiceExpireTime* data = reinterpret_cast<TServiceExpireTime*>(packet->data());
		data->expire_time_ = expire_time;

		packet->setDataSizeWithoutHeader(sizeof(TServiceExpireTime));

		return packet;
	}

	inline rnPacket* openMissionMap(tINT map_id, tINT type, tINT min_level, tINT max_level, tINT max_players)
	{
		rnPacket* packet = new rnPacket(sizeof(TOpenMissionMap));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_OPEN_MISSION_MAP);

		TOpenMissionMap* data = reinterpret_cast<TOpenMissionMap*>(packet->data());
		data->map_id_ = map_id;
		data->type_ = type;
		data->min_level_ = min_level;
		data->max_level_ = max_level;
		data->max_players_ = max_players;
		packet->setDataSizeWithoutHeader(sizeof(TOpenMissionMap));
		return packet;
	}

	inline rnPacket* closeMissionMap(tINT map_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TCloseMissionMap));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_CLOSE_MISSION_MAP);

		TCloseMissionMap* data = reinterpret_cast<TCloseMissionMap*>(packet->data());
		data->map_id_ = map_id;
		packet->setDataSizeWithoutHeader(sizeof(TCloseMissionMap));
		return packet;
	}

	inline rnPacket* joinMissionMapReq(tINT from_map_id, tINT from_channel_id, tINT to_map_id, tINT to_channel_id, tINT actor_id, tINT actor_level,
		coin_t actor_coin, fame_t actor_fame)
	{
		rnPacket* packet = new rnPacket(sizeof(TJoinMissionMapReq));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_JOIN_MISSION_MAP_REQ);

		TJoinMissionMapReq* data = reinterpret_cast<TJoinMissionMapReq*>(packet->data());
		data->from_map_id_ = from_map_id;
		data->from_channel_id_ = from_channel_id;
		data->to_map_id_ = to_map_id;
		data->to_channel_id_ = to_channel_id;
		data->actor_id_ = actor_id;
		data->actor_level_ = actor_level;
		data->actor_coin_ = actor_coin;
		data->actor_fame_ = actor_fame;
		packet->setDataSizeWithoutHeader(sizeof(TJoinMissionMapReq));
		return packet;
	}

	inline rnPacket* removeQueueMissionMap(tINT actor_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TRemoveQueueMissionMap));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_REMOVE_QUEUE_MISSION_MAP);

		TRemoveQueueMissionMap* data = reinterpret_cast<TRemoveQueueMissionMap*>(packet->data());
		data->actor_id_ = actor_id;
		packet->setDataSizeWithoutHeader(sizeof(TRemoveQueueMissionMap));
		return packet;
	}

	inline rnPacket* pcbangFlag(tINT actor_id, tBOOL pcbang_flag)
	{
		rnPacket* packet = new rnPacket(sizeof(TPcbangFlag));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_PCBANG_FLAG);

		TPcbangFlag* data = reinterpret_cast<TPcbangFlag*>(packet->data());
		data->actor_id_ = actor_id;
		data->pcbang_flag_ = pcbang_flag;
		packet->setDataSizeWithoutHeader(sizeof(TPcbangFlag));
		return packet;
	}

	inline rnPacket* serviceGroupLock(tBYTE group_id, tBOOL is_lock)
	{
		rnPacket* packet = new rnPacket(sizeof(TServiceGroupLock));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_SERVICE_GROUP_LOCK);

		TServiceGroupLock data;
		data.group_id_ = group_id;
		data.is_lock_ = is_lock;

		packet->addVALUE<TServiceGroupLock>(data);
		return packet;
	}

	inline rnPacket* createInstance(TCreateInstance& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TCreateInstance));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_CREATE_INSTANCE);
		packet->addVALUE<TCreateInstance>(data);
		return packet;
	}

	inline rnPacket* destroyInstance(TDestroyInstance& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TDestroyInstance));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_DESTROY_INSTANCE);
		packet->addVALUE<TDestroyInstance>(data);
		return packet;
	}

	inline rnPacket* bindInstance(TBindInstance& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TBindInstance));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_BIND_INSTANCE);
		packet->addVALUE<TBindInstance>(data);
		return packet;
	}

	inline rnPacket* unbindUserInstance(TUnBindUserInstance& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TUnBindUserInstance));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_UNBIND_USER_INSTANCE);
		packet->addVALUE<TUnBindUserInstance>(data);
		return packet;
	}

	inline rnPacket* unbindPartyInstance(TUnBindPartyInstance& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TUnBindPartyInstance));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_UNBIND_PARTY_INSTANCE);
		packet->addVALUE<TUnBindPartyInstance>(data);
		return packet;
	}

	inline rnPacket* queryBoundInstace(const TQueryBoundInstance& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TQueryBoundInstance));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_QUERY_BOUND_INSTANCE);
		packet->addVALUE<TQueryBoundInstance>(data);
		return packet;
	}

	inline rnPacket* billingServiceLock(tBOOL is_lock)
	{
		rnPacket* packet = new rnPacket(sizeof(TBillingServiceLock));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_BILLING_SERVICE_LOCK);

		TBillingServiceLock* data = reinterpret_cast<TBillingServiceLock*>(packet->data());
		data->is_lock_ = is_lock;
		packet->setDataSizeWithoutHeader(sizeof(TBillingServiceLock));
		return packet;
	}

	inline rnPacket* autouserBan(tINT actor_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TAutouserBan));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_AUTOUSER_BAN);

		TAutouserBan* data = reinterpret_cast<TAutouserBan*>(packet->data());
		data->actor_id_ = actor_id;
		packet->setDataSizeWithoutHeader(sizeof(TAutouserBan));
		return packet;
	}

	inline rnPacket* autoUserQuestionFastPacket(tINT actor_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TAutoUserQuestionFastPacket));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_AUTOUSER_QUESTION_FASTPACKET);

		TAutoUserQuestionFastPacket* data = reinterpret_cast<TAutoUserQuestionFastPacket*>(packet->data());
		data->actor_id_ = actor_id;
		packet->setDataSizeWithoutHeader(sizeof(TAutoUserQuestionFastPacket));
		return packet;
	}


	inline rnPacket* changeServerSetting(tFLOAT kill_mob_exp_rate)
	{
		rnPacket* packet = new rnPacket(sizeof(TChangeServerSetting));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_CHANGE_SERVER_SETTING);

		TChangeServerSetting* data = reinterpret_cast<TChangeServerSetting*>(packet->data());
		data->kill_mob_exp_rate_ = kill_mob_exp_rate;
		packet->setDataSizeWithoutHeader(sizeof(TChangeServerSetting));
		return packet;
	}

	inline rnPacket* addPacketLog(tINT user_index)
	{
		rnPacket* packet = new rnPacket(sizeof(TAddPacketLog));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_ADD_PACKET_LOG);

		TAddPacketLog* data = reinterpret_cast<TAddPacketLog*>(packet->data());
		data->user_index_ = user_index;
		packet->setDataSizeWithoutHeader(sizeof(TAddPacketLog));
		return packet;
	}

	inline rnPacket* removePacketLog(tINT user_index)
	{
		rnPacket* packet = new rnPacket(sizeof(TRemovePacketLog));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_REMOVE_PACKET_LOG);

		TRemovePacketLog* data = reinterpret_cast<TRemovePacketLog*>(packet->data());
		data->user_index_ = user_index;
		packet->setDataSizeWithoutHeader(sizeof(TRemovePacketLog));
		return packet;
	}

	inline rnPacket* kickLoginServerUser(tINT login_type, const tCHAR* user_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TKickLoginServerUser));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_KICK_LOGIN_SERVER_USER);

		TKickLoginServerUser* data = reinterpret_cast<TKickLoginServerUser*>(packet->data());
		data->login_type_ = login_type;
		memcpy(data->user_id_, user_id, sizeof(data->user_id_));
		packet->setDataSizeWithoutHeader(sizeof(TKickLoginServerUser));
		return packet;
	}

	inline rnPacket* kickProxyServerUser(tINT user_index)
	{
		rnPacket* packet = new rnPacket(sizeof(TKickProxyServerUser));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_KICK_PROXY_SERVER_USER);

		TKickProxyServerUser* data = reinterpret_cast<TKickProxyServerUser*>(packet->data());
		data->user_index_ = user_index;
		packet->setDataSizeWithoutHeader(sizeof(TKickProxyServerUser));
		return packet;
	}

	inline rnPacket* postEvent(const tCHAR* sender_name, const tCHAR* title, const tCHAR* content, tINT coin, TOwnItem* item,
		tINT vaild_time, tBYTE mail_type, tINT min_level, tINT max_level)
	{
		rnPacket* packet = new rnPacket(sizeof(TPostEvent));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_POST_EVENT);
		TPostEvent* data = reinterpret_cast<TPostEvent*>(packet->data());
		STRNCPY(data->sender_name_, sender_name, STATUS_NAME_MAX);
		STRNCPY(data->title_, title, MAIL_TITLE_STRING_MAX);
		STRNCPY(data->content_, content, MAIL_CONTENT_STRING_MAX);
		data->coin_ = coin;

		if (item)
		{
			memcpy(&data->item_, item, sizeof(data->item_));
		}
		else
		{
			memset(&data->item_, 0x00, sizeof(data->item_));
		}

		data->valid_time_ = vaild_time;
		data->mail_type_ = mail_type;
		data->min_level_ = min_level;
		data->max_level_ = max_level;


		packet->setDataSizeWithoutHeader(sizeof(TPostEvent));

		return packet;
	}

	inline rnPacket* recallPostEvent()
	{
		rnPacket* packet = new rnPacket(sizeof(TRecallPostEvent));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_RECALL_POST_EVENT);
		return packet;
	}

	inline rnPacket* completedOneDayQuestInfo(tINT user_index, tINT actor_id, completed_quest_vec_t& comepletedOneDayQuest)
	{
		rnPacket* packet = new rnPacket(sizeof(TCompletedOneDayQuest) + (sizeof(tSINT) * comepletedOneDayQuest.size()));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_COMPLETED_ONEDAY_QUEST_INFO);
		TCompletedOneDayQuest* data = reinterpret_cast<TCompletedOneDayQuest*>(packet->data());
		data->user_idnex_ = user_index;
		data->actor_id_ = actor_id;
		data->count_ = (tSINT)comepletedOneDayQuest.size();
		packet->setDataSizeWithoutHeader(sizeof(TCompletedOneDayQuest));

		//		completed_quest_vec_t::iterator begin = comepletedOneDayQuest.begin();
		//		completed_quest_vec_t::iterator end = comepletedOneDayQuest.end();
		for (tINT i = 0; i < (tINT)comepletedOneDayQuest.size(); i++)
		{
			packet->addSINT(comepletedOneDayQuest[i]);
		}

		return packet;
	}

	inline rnPacket* resetInstance(tINT user_id, tINT party_id, tINT map_id, tINT difficulty, tINT remain_reset_count,
		tINT inven_index)
	{
		rnPacket* packet = new rnPacket(sizeof(TResetInstance));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_RESET_INSTANCE);
		TResetInstance* data = reinterpret_cast<TResetInstance*>(packet->data());
		data->user_id_ = user_id;
		data->party_id_ = party_id;
		data->map_id_ = map_id;
		data->difficulty_ = difficulty;
		data->remain_reset_count_ = remain_reset_count;
		data->inven_index_ = inven_index;
		packet->setDataSizeWithoutHeader(sizeof(TResetInstance));
		return packet;
	}

	inline rnPacket* resetInstanceAll(tINT user_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TResetInstanceAll));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_RESET_INSTANCE_ALL);
		TResetInstanceAll* data = reinterpret_cast<TResetInstanceAll*>(packet->data());
		data->user_id_ = user_id;
		packet->setDataSizeWithoutHeader(sizeof(TResetInstanceAll));
		return packet;
	}

	inline rnPacket* getInstnaceCooltime(tINT user_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TGetInstanceCooltime));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_INSTANCE_COOLTIME_LIST);
		TGetInstanceCooltime* data = reinterpret_cast<TGetInstanceCooltime*>(packet->data());
		data->user_id_ = user_id;
		packet->setDataSizeWithoutHeader(sizeof(TGetInstanceCooltime));
		return packet;
	}

	inline rnPacket* resetInstnaceCooltimeAll(tINT user_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TResetInstanceCooltimeAll));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_RESET_INSTANCE_COOLTIME_ALL);
		TResetInstanceCooltimeAll* data = reinterpret_cast<TResetInstanceCooltimeAll*>(packet->data());
		data->user_id_ = user_id;
		packet->setDataSizeWithoutHeader(sizeof(TResetInstanceCooltimeAll));
		return packet;
	}

	inline rnPacket* startDestroyInstance(tINT map_id, tINT channel_id, tINT instance_id, tINT unload_time)
	{
		rnPacket* packet = new rnPacket(sizeof(TStartDestroyInstance));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_START_DESTROY_INSTANCE);
		TStartDestroyInstance* data = reinterpret_cast<TStartDestroyInstance*>(packet->data());
		data->map_id_ = map_id;
		data->channel_id_ = channel_id;
		data->instance_id_ = instance_id;
		data->unload_time_ = unload_time;
		packet->setDataSizeWithoutHeader(sizeof(TStartDestroyInstance));
		return packet;
	}

	inline rnPacket* cancelDestroyInstance(tINT map_id, tINT channel_id, tINT instance_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TCancelDestroyInstance));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_CANCEL_DESTROY_INSTANCE);
		TCancelDestroyInstance* data = reinterpret_cast<TCancelDestroyInstance*>(packet->data());
		data->map_id_ = map_id;
		data->channel_id_ = channel_id;
		data->instance_id_ = instance_id;
		packet->setDataSizeWithoutHeader(sizeof(TCancelDestroyInstance));
		return packet;
	}

	inline rnPacket* centerInMap(tINT user_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TCenterInMap));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_CENTER_INMAP);
		TCenterInMap* data = reinterpret_cast<TCenterInMap*>(packet->data());
		data->user_id_ = user_id;
		packet->setDataSizeWithoutHeader(sizeof(TCenterInMap));
		return packet;
	}

	inline rnPacket* centerOutMap(tINT user_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TCenterOutMap));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_CENTER_OUTMAP);
		TCenterOutMap* data = reinterpret_cast<TCenterOutMap*>(packet->data());
		data->user_id_ = user_id;
		packet->setDataSizeWithoutHeader(sizeof(TCenterOutMap));
		return packet;
	}

	inline rnPacket* reloadCreateLimit()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_RELOAD_CREATE_LIMIT);
		return packet;
	}

	inline rnPacket* updateEventList()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_UPDATE_EVENT_LIST);
		return packet;
	}

	inline rnPacket* changeActorName(tINT actor_id, const tCHAR* new_name)
	{
		rnPacket* packet = new rnPacket(sizeof(TChangeActorName));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_CHANGE_ACTOR_NAME);
		TChangeActorName* data = reinterpret_cast<TChangeActorName*>(packet->data());
		data->actor_id_ = actor_id;
		STRNCPY(data->new_name_, new_name, sizeof(data->new_name_));
		packet->setDataSizeWithoutHeader(sizeof(TChangeActorName));
		return packet;
	}

	inline rnPacket* realtimeEventStatus(tBOOL status)
	{
		rnPacket* packet = new rnPacket(sizeof(TRealtimeEventStatus));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_REALTIME_EVENT_STATUS);
		TRealtimeEventStatus* data = reinterpret_cast<TRealtimeEventStatus*>(packet->data());
		data->status_ = status;
		packet->setDataSizeWithoutHeader(sizeof(TRealtimeEventStatus));
		return packet;
	}

	inline rnPacket* serverCommandUpdate()
	{
		rnPacket* packet = new rnPacket(sizeof(TServerCommandUpdate));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_SERVER_COMMAND_UPDATE);
		return packet;
	}

	inline rnPacket* raidMobRegenStatus(tINT actor_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TRaidMobRegenStatus));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_RAID_MOB_REGEN_STATUS);
		TRaidMobRegenStatus* send_data = reinterpret_cast<TRaidMobRegenStatus*>(packet->data());
		send_data->actor_id_ = actor_id;
		packet->setDataSizeWithoutHeader(sizeof(TRaidMobRegenStatus));
		return packet;
	}

	inline rnPacket* sendScheduleMail(tINT server_group, tINT schedule_no)
	{
		rnPacket* packet = new rnPacket(sizeof(TSendScheduleMail));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_SEND_SCHEDULE_MAIL);
		TSendScheduleMail* send_data = reinterpret_cast<TSendScheduleMail*>(packet->data());
		send_data->server_group_ = server_group;
		send_data->schedule_no_ = schedule_no;
		packet->setDataSizeWithoutHeader(sizeof(TSendScheduleMail));
		return packet;
	}

	inline rnPacket* sendErrorETimedOut(tINT server_type, tINT group_id, tINT id)
	{
		rnPacket* packet = new rnPacket(sizeof(TErrorETimedOut));
		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_ERROR_ETIMEDOUT);
		TErrorETimedOut* send_data = reinterpret_cast<TErrorETimedOut*>(packet->data());
		send_data->server_type_ = server_type;
		send_data->group_id_ = group_id;
		send_data->id_ = id;
		packet->setDataSizeWithoutHeader(sizeof(TErrorETimedOut));
		return packet;
	}
}
