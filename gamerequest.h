#pragma once

namespace GameRequest
{
#pragma pack(push, 1)

	struct GameRequestHeader
	{
		tINT instance_id_;
	};

	struct TAddActorToMob : public GameRequestHeader
	{
		tBYTE			actor_sort_;
		tINT			actor_id_;
		tCHAR			name_[STATUS_NAME_MAX];
		tBYTE			kind_;
		tSINT			job_;
		tFLOAT			x_;
		tFLOAT			z_;
		tBYTE			team_;
		tINT			guild_id_;
	};

	struct TRemoveActor : public GameRequestHeader
	{
		tBYTE		actor_sort_;
		tINT		actor_id_;
	};

	// gc -> gm
	struct TAddMissionMob : public GameRequestHeader
	{
		tINT		script_id_;
		tINT		mission_id_;
		tINT		zone_;
		tFLOAT		left_;
		tFLOAT		top_;
		tFLOAT		right_;
		tFLOAT		bottom_;
		tINT		count_;
		tINT		point_;
		tINT		group_;
		tBOOL		regenable_;
	};

	struct TRemoveZoneMissionMob : public GameRequestHeader
	{
		tINT		zone_;
	};

	struct TKnockBackMove : public GameRequestHeader
	{
		tBYTE	attacker_sort_;
		tINT	attacker_id_;
		tBYTE	target_sort_;
		tINT	target_id_;
		tINT	skill_id_;
		tFLOAT	dest_x_;
		tFLOAT	dest_z_;
		tFLOAT	direction_;
		tFLOAT	speed_;
	};

	struct TSummonMapEventMob : public GameRequestHeader
	{
		tINT		event_id_;
		tINT		lifetime_;
		tINT		actor_id_;
	};

	// 공격 패킷
	struct TAttackTarget : public GameRequestHeader
	{
		tBYTE			actor_sort_;
		tINT			actor_id_;
		tINT			skill_id_;
		tINT			target_count_;		// 최대 20
		struct tagTarget
		{
			tBYTE		sort_;
			tINT		id_;
		} list_[0];
	};

	struct TAttackSelf : public GameRequestHeader
	{
		tBYTE			actor_sort_;
		tINT			actor_id_;
		tINT			skill_id_;
	};

	// 무적 버프
	struct TSetInvulnerabilityAffect : public GameRequestHeader
	{
		tINT	actor_id_;
	};

	struct TModifyAggroPercent : public GameRequestHeader
	{
		tINT	actor_id_;
		tINT	modify_aggro_percent_;
	};

	struct THiddenStatus : public GameRequestHeader
	{
		tBYTE	actor_sort_;
		tINT	actor_id_;
		tBOOL	on_off_;
		tINT	hidden_level_;
		tINT	hidden_detect_level_;
	};

	struct TStopForTalk : public GameRequestHeader
	{
		tINT	actor_id_;
	};

	struct TPetSummon : public GameRequestHeader
	{
		tBYTE	actor_sort_;
		tINT	actor_id_;
		tINT	skill_id_;
		tBYTE	target_sort_;
		tINT	target_id_;
	};

	struct TCreateInstanceMap : public GameRequestHeader
	{
		tINT	difficulty_;
	};

	struct TDestroyInstanceMap : public GameRequestHeader
	{
	};

	struct TResetKillExpiredTime
	{
		tINT	instance_id_;
		tINT	mob_id_;
	};

	struct TAddMgtMissonMob : public GameRequestHeader
	{
		tINT		guid_;
		tINT		mission_id_;
		tINT		zone_;
		tINT		level_;
		tBOOL		regenable_;
	};

	struct TAddAggro : public GameRequestHeader
	{
		tINT		mob_id_;
		tINT		target_id_;
		tINT		aggro_;
	};

	struct TRescueMob : public GameRequestHeader
	{
		tINT		user_id_;
		tINT		mob_id_;
	};

	struct TChangeStoneRegenable : public GameRequestHeader
	{
	};

	struct TRespawnUniqueMob
	{
		tINT		guid_;
	};

	struct TLeaveBattleSummonMob : public GameRequestHeader
	{
		tINT user_id_;
	};

	struct TActorPetSummon : public GameRequestHeader
	{
		tINT		actor_id_;
		tINT		pet_script_id_;
		tINT		pet_level_script_id_;
		tBYTE		command_type_;
		tINT		current_hp_;
		tINT		current_mp_;
	};

	struct TActorPetCommand : public GameRequestHeader
	{
		tINT		actor_id_;
		tINT		command_type_;
	};

	struct TChangeActorName : public GameRequestHeader
	{
		tINT actor_id_;
		tCHAR new_name_[STATUS_NAME_MAX];
	};

#pragma pack(pop)


	// function
	inline rnPacket* addActorToMob(tINT instance_id, tBYTE actor_sort, tINT actor_id, const tCHAR* name, tBYTE kind, tSINT job,
		tFLOAT x, tFLOAT z, tBYTE team, tINT guild_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TAddActorToMob));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_ADDACTOR_TOMOB);
		TAddActorToMob*	add_actor = reinterpret_cast<TAddActorToMob*>(packet->data());

		add_actor->instance_id_ = instance_id;
		add_actor->actor_sort_ = actor_sort;
		add_actor->actor_id_ = actor_id;
		if (name)
			memcpy(add_actor->name_, name, sizeof(add_actor->name_));
		else
			memset(add_actor->name_, 0, sizeof(add_actor->name_));
		add_actor->kind_ = kind;
		add_actor->job_ = job;
		add_actor->x_ = x;
		add_actor->z_ = z;
		add_actor->team_ = team;
		add_actor->guild_id_ = guild_id;
		packet->setDataSizeWithoutHeader(sizeof(TAddActorToMob));

		return packet;
	}

	inline rnPacket* removeActor(tINT instance_id, tBYTE actor_sort, tINT actor_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TRemoveActor));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_REMOVEACTOR);
		TRemoveActor* remove_actor = reinterpret_cast<TRemoveActor*>(packet->data());

		remove_actor->instance_id_ = instance_id;
		remove_actor->actor_sort_ = actor_sort;
		remove_actor->actor_id_ = actor_id;
		packet->setDataSizeWithoutHeader(sizeof(TRemoveActor));

		return packet;
	}

	inline rnPacket* castSkill(TAttackTarget& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TAttackTarget));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_CAST_SKILL);
		packet->addVALUE<TAttackTarget>(data);
		return packet;
	}

	inline rnPacket* castSkillSelf(TAttackSelf& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TAttackSelf));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_CAST_SKILL_SELF);
		packet->addVALUE<TAttackSelf>(data);
		return packet;
	}

	inline rnPacket* addMissionMob(tINT instance_id, tINT script_id, tINT mission_id, tINT zone, tFLOAT left, tFLOAT top, tFLOAT right, tFLOAT bottom,
		tINT count, tINT point, tINT group, tBOOL regenable)
	{
		rnPacket* packet = new rnPacket(sizeof(TAddMissionMob));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_ADD_MISSION_MOB);
		TAddMissionMob* add_mission_mob = reinterpret_cast<TAddMissionMob*>(packet->data());
		add_mission_mob->instance_id_ = instance_id,
			add_mission_mob->script_id_ = script_id;
		add_mission_mob->mission_id_ = mission_id;
		add_mission_mob->zone_ = zone;
		add_mission_mob->left_ = left;
		add_mission_mob->top_ = top;
		add_mission_mob->right_ = right;
		add_mission_mob->bottom_ = bottom;
		add_mission_mob->count_ = count;
		add_mission_mob->point_ = point;
		add_mission_mob->group_ = group;
		add_mission_mob->regenable_ = regenable;
		packet->setDataSizeWithoutHeader(sizeof(TAddMissionMob));

		return packet;
	}

	inline rnPacket* removeZoneMissionMob(tINT instance_id, tINT zone)
	{
		rnPacket* packet = new rnPacket(sizeof(TRemoveZoneMissionMob));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_REMOVE_ZONE_MISSION_MOB);
		TRemoveZoneMissionMob* remove_zone_mission_mob = reinterpret_cast<TRemoveZoneMissionMob*>(packet->data());

		remove_zone_mission_mob->instance_id_ = instance_id;
		remove_zone_mission_mob->zone_ = zone;
		packet->setDataSizeWithoutHeader(sizeof(TRemoveZoneMissionMob));

		return packet;
	}

	inline rnPacket* knockBackMove(TKnockBackMove& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TKnockBackMove));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_ATTACK_KNOCKBACKMOVE);
		packet->addVALUE<TKnockBackMove>(data);

		return packet;
	}

	inline rnPacket* summonMapEventMob(tINT instance_id, tINT event_id, tINT lifetime, tINT actor_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TSummonMapEventMob));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_SUMMON_MAP_EVENT_MOB);
		TSummonMapEventMob* summon_map_event_mob = reinterpret_cast<TSummonMapEventMob*>(packet->data());
		summon_map_event_mob->instance_id_ = instance_id;
		summon_map_event_mob->event_id_ = event_id;
		summon_map_event_mob->lifetime_ = lifetime;
		summon_map_event_mob->actor_id_ = actor_id;
		packet->setDataSizeWithoutHeader(sizeof(TSummonMapEventMob));

		return packet;
	};

	inline rnPacket* setInvulnerabilityAffect(TSetInvulnerabilityAffect& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TSetInvulnerabilityAffect));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_SET_INVULNERABILITY_AFFECT);
		packet->addVALUE<TSetInvulnerabilityAffect>(data);

		return packet;
	}

	inline rnPacket* modifyAggroPercent(TModifyAggroPercent& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TModifyAggroPercent));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_MODIFY_AGGRO_PERCENT);
		packet->addVALUE<TModifyAggroPercent>(data);

		return packet;
	}

	inline rnPacket* hiddenStatus(THiddenStatus& data)
	{
		rnPacket* packet = new rnPacket(sizeof(THiddenStatus));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_HIDDEN_STATUS);
		packet->addVALUE<THiddenStatus>(data);
		return packet;
	}

	inline rnPacket* stopForTalk(TStopForTalk& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TStopForTalk));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_STOP_FOR_TALK);
		packet->addVALUE<TStopForTalk>(data);
		return packet;
	}

	inline rnPacket* petSummon(TPetSummon& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TPetSummon));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_PET_SUMMON);
		packet->addVALUE<TPetSummon>(data);
		return packet;
	}

	inline rnPacket* createInstanceMap(TCreateInstanceMap& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TCreateInstanceMap));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_CREATE_INSTANCE_MAP);
		packet->addVALUE<TCreateInstanceMap>(data);
		return packet;
	}

	inline rnPacket* destroyInstanceMap(TDestroyInstanceMap& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TDestroyInstanceMap));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_DESTROY_INSTANCE_MAP);
		packet->addVALUE<TDestroyInstanceMap>(data);
		return packet;
	}

	inline rnPacket* resetKillExpiredTime(tINT instance_id, tINT mob_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TResetKillExpiredTime));

		TResetKillExpiredTime data;
		data.instance_id_ = instance_id;
		data.mob_id_ = mob_id;

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_RESET_KILL_EXPIRED_TIME);
		packet->addVALUE<TResetKillExpiredTime>(data);
		return packet;
	}

	inline rnPacket* addMgtMissionMob(TAddMgtMissonMob& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TAddMgtMissonMob));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_ADD_MGT_MISSION_MOB);
		packet->addVALUE<TAddMgtMissonMob>(data);
		return packet;
	}

	inline rnPacket* addAggro(TAddAggro& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TAddAggro));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_ADD_AGGRO);
		packet->addVALUE<TAddAggro>(data);
		return packet;
	}

	inline rnPacket* rescueMob(TRescueMob& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TRescueMob));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_RESCUE_MOB);
		packet->addVALUE<TRescueMob>(data);
		return packet;
	}

	inline rnPacket* changeStoneRegenable(TChangeStoneRegenable& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TChangeStoneRegenable));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_CHANGE_STONE_REGENABLE);
		packet->addVALUE<TChangeStoneRegenable>(data);
		return packet;
	}

	inline rnPacket* respawnUniqueMob(tINT guid)
	{
		rnPacket* packet = new rnPacket(sizeof(TRespawnUniqueMob));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_RESPAWN_UNIQUE_MOB);
		TRespawnUniqueMob* data = reinterpret_cast<TRespawnUniqueMob*>(packet->data());
		data->guid_ = guid;
		packet->setDataSizeWithoutHeader(sizeof(TRespawnUniqueMob));

		return packet;
	}

	inline rnPacket* leaveBattleSummonMob(tINT instance_id, tINT user_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TLeaveBattleSummonMob));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_LEAVE_BATTLE_SUMMON_MOB);
		TLeaveBattleSummonMob* data = reinterpret_cast<TLeaveBattleSummonMob*>(packet->data());
		data->instance_id_ = instance_id;
		data->user_id_ = user_id;
		packet->setDataSizeWithoutHeader(sizeof(TLeaveBattleSummonMob));

		return packet;
	}

	inline rnPacket* actorPetSummon(TActorPetSummon& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TActorPetSummon));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_ACTOR_PET_SUMMON);
		packet->addVALUE<TActorPetSummon>(data);
		return packet;
	}

	inline rnPacket* actorPetCommand(TActorPetCommand& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TActorPetCommand));

		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_ACTOR_PET_COMMAND);
		packet->addVALUE<TActorPetCommand>(data);
		return packet;
	}

	inline rnPacket* changeActorName(tINT instance_id, tINT actor_id, const tCHAR* new_name)
	{
		rnPacket* packet = new rnPacket(sizeof(TChangeActorName));
		packet->setGroupType(PGROUP_GAME_REQUEST, PTYPE_GAME_CHANGE_ACTOR_NAME);
		TChangeActorName* data = reinterpret_cast<TChangeActorName*>(packet->data());
		data->instance_id_ = instance_id;
		data->actor_id_ = actor_id;
		STRNCPY(data->new_name_, new_name, sizeof(data->new_name_));
		packet->setDataSizeWithoutHeader(sizeof(TChangeActorName));
		return packet;
	}
}
