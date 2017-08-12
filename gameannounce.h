#pragma once

namespace GameAnnounce
{
#pragma pack(push, 1)

	struct GameAnnounceHeader
	{
		tINT instance_id_;
	};

	struct TMoveActor : public GameAnnounceHeader
	{
		tBYTE		actor_sort_;
		tINT		actor_id_;
		tFLOAT		direction_;
		tFLOAT		x_;
		tFLOAT		z_;
		tBYTE		move_flags_;
		tFLOAT		speed_;
		tBYTE		state_;
		tINT		hp_;
		tINT		mp_;
		tSINT		sp_;
		tUINT		move_wait_sec_;
		tBYTE		wait_ani_index_;
	};

	struct TMoveTurnActor : public GameAnnounceHeader
	{
		tBYTE		actor_sort_;
		tINT		actor_id_;
		tFLOAT		direction_;
	};

	struct TUpdateActorDead : public GameAnnounceHeader
	{
		tBYTE		actor_sort_;
		tINT		actor_id_;
		tINT		state_;
	};

	struct TUpdateActorRevive : public GameAnnounceHeader
	{
		tBYTE		sort_;
		tINT		actor_id_;
		tSINT		state_;
		tFLOAT		x_;
		tFLOAT		z_;
	};

	struct TUpdateActorInfo : public GameAnnounceHeader
	{
		tBYTE			actor_sort_;
		tINT			actor_id_;
		tSINT			job_;
		tBYTE			kind_;
		tSINT			level_;
		tINT			party_id_;
		tINT			max_hp_;
		tINT			max_mp_;
		tINT			max_sp_;
		tBYTE			pk_state_;
		tINT			pet_level_script_id_;
		tINT			guild_id_;
	};

	struct TRecallMob : public GameAnnounceHeader
	{
		tINT		mob_id_;
		tFLOAT		mob_x_;
		tFLOAT		mob_z_;
	};

	struct TCasino : public GameAnnounceHeader
	{
		tBYTE		state_;
	};

	// gm -> gc
	struct TCreateMob : public GameAnnounceHeader
	{
		tINT		mob_id_;
		tINT		script_id_;
		tFLOAT		x_;
		tFLOAT		z_;
		tFLOAT		direction_;
		tINT		mission_id_;
		tINT		mission_zone_;
		tINT		point_;
		tINT		group_;
		tINT		guild_id_;
		tINT		map_event_actor_id_;
		tINT		map_event_id_;
		tINT		orignal_id_;
		tBYTE		gm_summon_flag_;
	};

	// gc -> gm
	struct TDealDamage : public GameAnnounceHeader
	{
		tBYTE		attacker_sort_;
		tINT		attacker_id_;
		tBYTE		target_sort_;
		tINT		target_id_;
		tINT		damage_;
		tINT		aggro_;
		tINT		skill_id_;
	};

	// gc -> gm
	struct TDeadMob : public GameAnnounceHeader
	{
		tINT		mob_id_;
		tBYTE		killer_sort_;
		tINT		killer_id_;
		tBYTE		dead_state_;
	};

	// gm -> gc
	struct TDeleteMob : public GameAnnounceHeader
	{
		tINT		mob_id_;
		tINT		state_;
	};

	struct TAddAffect : public GameAnnounceHeader
	{
		tBYTE		actor_sort_;
		tINT		actor_id_;
		tINT		affect_id_;
	};

	struct TRemoveAffect : public GameAnnounceHeader
	{
		tBYTE		actor_sort_;
		tINT		actor_id_;
		tINT		affect_id_;
	};

	struct THealMob : public GameAnnounceHeader
	{
		tINT		mob_id_;
		tINT		heal_hp_;
		tINT		heal_mp_;
		tINT		heal_sp_;
	};

	struct TUpdatePowers : public GameAnnounceHeader
	{
		tBYTE		actor_sort_;
		tINT		actor_id_;
		tINT		hp_;
		tINT		mp_;
		tINT		sp_;
	};

	struct TMapEventSummonMob : public GameAnnounceHeader
	{
		tINT		actor_id_;
		tINT		mob_id_;
	};

	struct TEnterBattle : public GameAnnounceHeader
	{
		tINT		mob_id_;
		tBYTE		first_attack_;
	};

	struct TLeaveBattle : public GameAnnounceHeader
	{
		tINT		mob_id_;
	};

	struct TStartHint : public GameAnnounceHeader
	{
		tINT		mob_id_;
		tINT		hint_id_;
		tINT		hint_time_;
		tINT		hint_type_; // 일반, 반응
		tINT		hint_condition_type_; // 스킬 타입, 스킬 소트
		tINT		hint_condition_value_; // hint_condition_type_에 따라 조건 검사에 쓸 값

		tBYTE		target_sort_;
		tINT		target_id_;
	};

	struct TCancelHint : public GameAnnounceHeader
	{
		tINT		mob_id_;
	};

	struct TAttackReaction : public GameAnnounceHeader
	{
		tINT		mob_id_;
		tBYTE		target_sort_;
		tINT		target_id_;
	};

	struct TUserEventAttack : public GameAnnounceHeader
	{
		tINT		user_id_;
		tBYTE		target_sort_;
		tINT		target_id_;
	};

	struct TUserEventAttackedBy : public GameAnnounceHeader
	{
		tINT		user_id_;
		tBYTE		attacker_sort_;
		tINT		attacker_id_;
	};

	struct TMountRickshaw : public GameAnnounceHeader
	{
		tINT		user_id_;
		tINT		mount_mob_id_;
	};

	struct TUnmountRickshaw : public GameAnnounceHeader
	{
		tINT		user_id_;
	};

	struct TActorAnimation : public GameAnnounceHeader
	{
		tBYTE	actor_sort_;
		tINT	actor_id_;
		tINT	ani_id_;
		tBYTE	target_sort_;
		tINT	target_id_;
	};

	struct TCancelRescue : public GameAnnounceHeader
	{
		tBYTE	actor_sort_;
		tINT	actor_id_;
	};

	struct TWearMarkItem : public GameAnnounceHeader
	{
		tINT	user_id_;
		tINT	mark_item_id_;
	};

	struct TUnwearMarkItem : public GameAnnounceHeader
	{
		tINT	user_id_;
	};

	struct TUpdateGMHide : public GameAnnounceHeader
	{
		tBYTE		actor_sort_;
		tINT		actor_id_;
		tBOOL		hide_;
	};

	struct TDeadUniqueMob
	{
		tINT		guid_;
	};

	struct TWorldEventStatus
	{
		WorldEventTable world_event_table_;
	};

	struct TActionHelp : public GameAnnounceHeader
	{
		tINT		mob_id_;
	};

	struct TActionFleeing : public GameAnnounceHeader
	{
		tINT		mob_id_;
	};

	struct TRegionWarStatus
	{
		tBOOL is_war_;
	};

	struct TRegionWarCityInfo
	{
		tINT city_group_id_;
		tINT capture_guild_id_;
		tINT war_guild_count_;
		tINT war_guild_[0];
	};

	struct TAddWarGuild
	{
		tINT city_group_id_;
		tINT guild_id_;
	};

	struct TRemoveWarGuild
	{
		tINT city_group_id_;
		tINT guild_id_;
	};

	struct TCaptureCity
	{
		tINT city_group_id_;
		tINT guild_id_;
	};

	struct TResetTotem : public GameAnnounceHeader
	{
		tINT mob_id_;
	};

	struct TUpdateMaxHp : public GameAnnounceHeader
	{
		tINT mob_id_;
		tINT max_hp_;
	};

#pragma pack(pop)

	// function
	inline rnPacket* moveActor(TMoveActor &move_actor)
	{
		rnPacket* packet = new rnPacket(sizeof(TMoveActor));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_MOVEACTOR);
		packet->addVALUE<TMoveActor>(move_actor);
		return packet;
	}


	inline rnPacket* moveActor(tINT instance_id, tBYTE actor_sort, tINT actor_id, tFLOAT direction, tFLOAT x, tFLOAT y, tBYTE move_flags,
		tFLOAT speed, tBYTE state, tINT hp, tINT mp, tSINT sp, tUINT move_wait_sec, tBYTE wait_ani_index)
	{
		rnPacket* packet = new rnPacket(sizeof(TMoveActor));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_MOVEACTOR);
		TMoveActor*	move_actor = reinterpret_cast<TMoveActor*>(packet->data());
		move_actor->instance_id_ = instance_id;
		move_actor->actor_sort_ = actor_sort;
		move_actor->actor_id_ = actor_id;
		move_actor->direction_ = direction;
		move_actor->x_ = x;
		move_actor->z_ = y;
		move_actor->move_flags_ = move_flags;
		move_actor->speed_ = speed;
		move_actor->state_ = state;
		move_actor->hp_ = hp;
		move_actor->mp_ = mp;
		move_actor->sp_ = sp;
		move_actor->move_wait_sec_ = move_wait_sec;
		move_actor->wait_ani_index_ = wait_ani_index;
		packet->setDataSizeWithoutHeader(sizeof(TMoveActor));

		return packet;
	}

	inline rnPacket* moveTurnActor(tINT instance_id, tBYTE actor_sort, tINT actor_id, tFLOAT direction)
	{
		rnPacket* packet = new rnPacket(sizeof(TMoveTurnActor));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_MOVE_TURN_ACTOR);
		TMoveTurnActor* send_data = reinterpret_cast<TMoveTurnActor*>(packet->data());

		send_data->instance_id_ = instance_id;
		send_data->actor_sort_ = actor_sort;
		send_data->actor_id_ = actor_id;
		send_data->direction_ = direction;
		packet->setDataSizeWithoutHeader(sizeof(TMoveTurnActor));

		return packet;
	}

	inline rnPacket* updateActorDead(tINT instance_id, tBYTE actor_sort, tINT actor_id, tINT state)
	{
		rnPacket* packet = new rnPacket(sizeof(TUpdateActorDead));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_UPDATEACTOR_DEAD);
		TUpdateActorDead* dead_actor = reinterpret_cast<TUpdateActorDead*>(packet->data());

		dead_actor->instance_id_ = instance_id;
		dead_actor->actor_sort_ = actor_sort;
		dead_actor->actor_id_ = actor_id;
		dead_actor->state_ = state;
		packet->setDataSizeWithoutHeader(sizeof(TUpdateActorDead));

		return packet;
	}

	inline rnPacket* updateActorRevive(tINT instance_id, tBYTE sort, tINT actor_id, tSINT state, tFLOAT x, tFLOAT z)
	{
		rnPacket* packet = new rnPacket(sizeof(TUpdateActorRevive));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_UPDATEACTOR_REVIVE);
		TUpdateActorRevive* revive_actor = reinterpret_cast<TUpdateActorRevive*>(packet->data());

		revive_actor->instance_id_ = instance_id;
		revive_actor->sort_ = sort;
		revive_actor->actor_id_ = actor_id;
		revive_actor->state_ = state;
		revive_actor->x_ = x;
		revive_actor->z_ = z;
		packet->setDataSizeWithoutHeader(sizeof(TUpdateActorRevive));

		return packet;
	}

	inline rnPacket* updateActorInfo(tINT instance_id, tBYTE actor_sort, tINT actor_id, tSINT job, tBYTE kind, tSINT level, tINT party_id,
		tINT max_hp, tINT max_mp, tINT max_sp, tBYTE pk_state, tINT pet_level_script_id, tINT guild_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TUpdateActorInfo));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_UPDATEACTOR_INFO);
		TUpdateActorInfo* actor_info = reinterpret_cast<TUpdateActorInfo*>(packet->data());

		actor_info->instance_id_ = instance_id;
		actor_info->actor_sort_ = actor_sort;
		actor_info->actor_id_ = actor_id;
		actor_info->job_ = job;
		actor_info->kind_ = kind;
		actor_info->level_ = level;
		actor_info->party_id_ = party_id;
		actor_info->max_hp_ = max_hp;
		actor_info->max_mp_ = max_mp;
		actor_info->max_sp_ = max_sp;
		actor_info->pk_state_ = pk_state;
		actor_info->pet_level_script_id_ = pet_level_script_id;
		actor_info->guild_id_ = guild_id;
		packet->setDataSizeWithoutHeader(sizeof(TUpdateActorInfo));

		return packet;
	}

	inline rnPacket* recallMob(tINT instance_id, tINT mob_id, tFLOAT mob_x, tFLOAT mob_z)
	{
		rnPacket* packet = new rnPacket(sizeof(TRecallMob));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_RECALLMOB);
		TRecallMob* recall_mob = reinterpret_cast<TRecallMob*>(packet->data());

		recall_mob->instance_id_ = instance_id;
		recall_mob->mob_id_ = mob_id;
		recall_mob->mob_x_ = mob_x;
		recall_mob->mob_z_ = mob_z;
		packet->setDataSizeWithoutHeader(sizeof(TRecallMob));

		return packet;
	}

	inline rnPacket* casino(tINT instance_id, tBYTE state)
	{
		rnPacket* packet = new rnPacket(sizeof(TCasino));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_CASINO);
		TCasino* casino = reinterpret_cast<TCasino*>(packet->data());

		casino->instance_id_ = instance_id;
		casino->state_ = state;
		packet->setDataSizeWithoutHeader(sizeof(TCasino));

		return packet;
	}

	inline rnPacket* createMob(tINT instance_id, tINT mob_id, tINT script_id, tFLOAT x, tFLOAT z, tFLOAT direction,
		tINT mission_id, tINT mission_zone, tINT point, tINT group, tINT guild_id, tINT map_event_actor_id, tINT map_event_id,
		tINT orignal_id, tBOOL gm_summon_flag)
	{
		rnPacket* packet = new rnPacket(sizeof(TCreateMob));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_CREATE_MOB);
		TCreateMob* create_mob = reinterpret_cast<TCreateMob*>(packet->data());

		create_mob->instance_id_ = instance_id;
		create_mob->mob_id_ = mob_id;
		create_mob->script_id_ = script_id;
		create_mob->x_ = x;
		create_mob->z_ = z;
		create_mob->direction_ = direction;
		create_mob->mission_id_ = mission_id;
		create_mob->mission_zone_ = mission_zone;
		create_mob->point_ = point;
		create_mob->group_ = group;
		create_mob->guild_id_ = guild_id;
		create_mob->map_event_actor_id_ = map_event_actor_id;
		create_mob->map_event_id_ = map_event_id;
		create_mob->orignal_id_ = orignal_id;
		create_mob->gm_summon_flag_ = gm_summon_flag;
		packet->setDataSizeWithoutHeader(sizeof(TCreateMob));

		return packet;
	}

	inline rnPacket* dealDamage(TDealDamage& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TDealDamage));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_DEAL_DAMAGE);
		packet->addVALUE<TDealDamage>(data);
		return packet;
	}

	inline rnPacket* deadMob(tINT instance_id, tINT mob_id, tBYTE killer_sort, tINT killer_id, tBYTE dead_state)
	{
		rnPacket* packet = new rnPacket(sizeof(TDeadMob));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_DEAD_MOB);
		TDeadMob* dead_mob = reinterpret_cast<TDeadMob*>(packet->data());

		dead_mob->instance_id_ = instance_id;
		dead_mob->mob_id_ = mob_id;
		dead_mob->killer_sort_ = killer_sort;
		dead_mob->killer_id_ = killer_id;
		dead_mob->dead_state_ = dead_state;
		packet->setDataSizeWithoutHeader(sizeof(TDeadMob));

		return packet;
	}

	inline rnPacket* deleteMob(tINT instance_id, tINT mob_id, tINT state)
	{
		rnPacket* packet = new rnPacket(sizeof(TDeleteMob));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_DELETE_MOB);
		TDeleteMob* delete_mob = reinterpret_cast<TDeleteMob*>(packet->data());

		delete_mob->instance_id_ = instance_id;
		delete_mob->mob_id_ = mob_id;
		delete_mob->state_ = state;
		packet->setDataSizeWithoutHeader(sizeof(TDeleteMob));

		return packet;
	}

	inline rnPacket* addAffect(tINT instance_id, tBYTE actor_sort, tINT actor_id, tINT affect_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TAddAffect));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_ADD_MOB_AFFECT);
		TAddAffect* data = reinterpret_cast<TAddAffect*>(packet->data());

		data->instance_id_ = instance_id;
		data->actor_sort_ = actor_sort;
		data->actor_id_ = actor_id;
		data->affect_id_ = affect_id;
		packet->setDataSizeWithoutHeader(sizeof(TAddAffect));

		return packet;
	}

	inline rnPacket* removeAffect(tINT instance_id, tBYTE actor_sort, tINT actor_id, tINT affect_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TRemoveAffect));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_REMOVE_MOB_AFFECT);
		TRemoveAffect* data = reinterpret_cast<TRemoveAffect*>(packet->data());

		data->instance_id_ = instance_id;
		data->actor_sort_ = actor_sort;
		data->actor_id_ = actor_id;
		data->affect_id_ = affect_id;
		packet->setDataSizeWithoutHeader(sizeof(TRemoveAffect));

		return packet;
	}

	inline rnPacket* healMob(tINT instance_id, tINT mob_id, tINT heal_hp, tINT heal_mp, tINT heal_sp)
	{
		rnPacket* packet = new rnPacket(sizeof(THealMob));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_HEAL_MOB);
		THealMob* data = reinterpret_cast<THealMob*>(packet->data());

		data->instance_id_ = instance_id;
		data->mob_id_ = mob_id;
		data->heal_hp_ = heal_hp;
		data->heal_mp_ = heal_mp;
		data->heal_sp_ = heal_sp;
		packet->setDataSizeWithoutHeader(sizeof(THealMob));

		return packet;
	};

	inline rnPacket* updatePowers(tINT instance_id, tBYTE actor_sort, tINT actor_id, tINT hp, tINT mp, tINT sp)
	{
		rnPacket* packet = new rnPacket(sizeof(TUpdatePowers));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_UPDATE_POWERS);
		TUpdatePowers* data = reinterpret_cast<TUpdatePowers*>(packet->data());

		data->instance_id_ = instance_id;
		data->actor_sort_ = actor_sort;
		data->actor_id_ = actor_id;
		data->hp_ = hp;
		data->mp_ = mp;
		data->sp_ = sp;
		packet->setDataSizeWithoutHeader(sizeof(TUpdatePowers));

		return packet;
	}

	inline rnPacket* mapEventSummonMob(tINT instance_id, tINT actor_id, tINT mob_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TMapEventSummonMob));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_SUMMON_MAP_EVENT_MOB);
		TMapEventSummonMob* data = reinterpret_cast<TMapEventSummonMob*>(packet->data());

		data->instance_id_ = instance_id;
		data->actor_id_ = actor_id;
		data->mob_id_ = mob_id;
		packet->setDataSizeWithoutHeader(sizeof(TMapEventSummonMob));

		return packet;
	}

	inline rnPacket* enterBattle(tINT instance_id, tINT mob_id, tBYTE first_attack)
	{
		rnPacket* packet = new rnPacket(sizeof(TEnterBattle));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_ENTER_BATTLE);
		TEnterBattle* data = reinterpret_cast<TEnterBattle*>(packet->data());

		data->instance_id_ = instance_id;
		data->mob_id_ = mob_id;
		data->first_attack_ = first_attack;
		packet->setDataSizeWithoutHeader(sizeof(TEnterBattle));

		return packet;
	}

	inline rnPacket* leaveBattle(tINT instance_id, tINT mob_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TLeaveBattle));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_LEAVE_BATTLE);
		TLeaveBattle* data = reinterpret_cast<TLeaveBattle*>(packet->data());

		data->instance_id_ = instance_id;
		data->mob_id_ = mob_id;
		packet->setDataSizeWithoutHeader(sizeof(TLeaveBattle));

		return packet;
	}

	inline rnPacket* startHint(TStartHint& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TStartHint));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_START_HINT);
		packet->addVALUE<TStartHint>(data);
		return packet;
	}

	inline rnPacket* cancelHint(TCancelHint& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TCancelHint));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_CANCEL_HINT);
		packet->addVALUE<TCancelHint>(data);
		return packet;
	}

	inline rnPacket* attackReaction(TAttackReaction& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TAttackReaction));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_ATTACK_REACTION);
		packet->addVALUE<TAttackReaction>(data);
		return packet;
	}

	inline rnPacket* userEventAttack(TUserEventAttack& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TUserEventAttack));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_USER_EVENT_ATTACK);
		packet->addVALUE<TUserEventAttack>(data);
		return packet;
	}

	inline rnPacket* userEventAttackedBy(TUserEventAttackedBy& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TUserEventAttackedBy));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_USER_EVENT_ATTACKED_BY);
		packet->addVALUE<TUserEventAttackedBy>(data);
		return packet;
	}

	inline rnPacket* mountRIckshaw(TMountRickshaw& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TMountRickshaw));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_MOUNT_RICKSHAW);
		packet->addVALUE<TMountRickshaw>(data);
		return packet;
	}

	inline rnPacket* unmountRIckshaw(TUnmountRickshaw& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TUnmountRickshaw));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_UNMOUNT_RICKSHAW);
		packet->addVALUE<TUnmountRickshaw>(data);
		return packet;
	}

	inline rnPacket* actorAnimation(TActorAnimation& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TActorAnimation));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_ACTOR_ANIMATION);
		packet->addVALUE<TActorAnimation>(data);
		return packet;
	}

	inline rnPacket* cancelRescue(TCancelRescue& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TCancelRescue));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_CANCEL_RESCUE);
		packet->addVALUE<TCancelRescue>(data);
		return packet;
	}

	inline rnPacket* wearMarkItem(TWearMarkItem& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TWearMarkItem));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_WEAR_MARK_ITEM);
		packet->addVALUE<TWearMarkItem>(data);
		return packet;
	}

	inline rnPacket* unwearMarkItem(TUnwearMarkItem& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TUnwearMarkItem));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_UNWEAR_MARK_ITEM);
		packet->addVALUE<TUnwearMarkItem>(data);
		return packet;
	}

	inline rnPacket* updateGMHide(TUpdateGMHide& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TUpdateGMHide));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_GM_HIDE);
		packet->addVALUE<TUpdateGMHide>(data);

		return packet;
	}

	inline rnPacket* deadUniqueMob(tINT guid)
	{
		rnPacket* packet = new rnPacket(sizeof(TDeadUniqueMob));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_DEAD_UNIQUE_MOB);
		TDeadUniqueMob* data = reinterpret_cast<TDeadUniqueMob*>(packet->data());
		data->guid_ = guid;
		packet->setDataSizeWithoutHeader(sizeof(TDeadUniqueMob));

		return packet;
	}

	inline rnPacket* worldEventStatus(WorldEventTable* world_event_table)
	{
		rnPacket* packet = new rnPacket(sizeof(TWorldEventStatus));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_WORLD_EVENT_STATUS);
		TWorldEventStatus* data = reinterpret_cast<TWorldEventStatus*>(packet->data());
		data->world_event_table_ = *world_event_table;
		packet->setDataSizeWithoutHeader(sizeof(TWorldEventStatus));

		return packet;
	}

	inline rnPacket* actionHelp(tINT instance_id, tINT mob_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TActionHelp));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_ACTION_HELP);
		TActionHelp* data = reinterpret_cast<TActionHelp*>(packet->data());
		data->instance_id_ = instance_id;
		data->mob_id_ = mob_id;
		packet->setDataSizeWithoutHeader(sizeof(TActionHelp));

		return packet;
	}

	inline rnPacket* actionFleeing(tINT instance_id, tINT mob_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TActionFleeing));

		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_ACTION_FLEEING);
		TActionFleeing* data = reinterpret_cast<TActionFleeing*>(packet->data());
		data->instance_id_ = instance_id;
		data->mob_id_ = mob_id;
		packet->setDataSizeWithoutHeader(sizeof(TActionFleeing));

		return packet;
	}

	inline rnPacket* regionWarStatus(tBOOL is_war)
	{
		rnPacket* packet = new rnPacket(sizeof(TRegionWarStatus));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_REGION_WAR_STATUS);
		TRegionWarStatus* data = reinterpret_cast<TRegionWarStatus*>(packet->data());
		data->is_war_ = is_war;
		packet->setDataSizeWithoutHeader(sizeof(TRegionWarStatus));
		return packet;
	}

	inline rnPacket* startRegionWar()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_START_REGION_WAR);
		return packet;
	}

	inline rnPacket* endRegionWar()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_END_REGION_WAR);
		return packet;
	}

	inline rnPacket* addWarGuild(tINT city_group_id, tINT guild_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TAddWarGuild));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_ADD_WAR_GUILD);
		TAddWarGuild* data = reinterpret_cast<TAddWarGuild*>(packet->data());
		data->city_group_id_ = city_group_id;
		data->guild_id_ = guild_id;
		packet->setDataSizeWithoutHeader(sizeof(TAddWarGuild));
		return packet;
	}

	inline rnPacket* removeWarGuild(tINT city_group_id, tINT guild_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TRemoveWarGuild));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_REMOVE_WAR_GUILD);
		TRemoveWarGuild* data = reinterpret_cast<TRemoveWarGuild*>(packet->data());
		data->city_group_id_ = city_group_id;
		data->guild_id_ = guild_id;
		packet->setDataSizeWithoutHeader(sizeof(TRemoveWarGuild));
		return packet;
	}

	inline rnPacket* captureCity(tINT city_group_id, tINT guild_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TCaptureCity));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_CAPTURE_CITY);
		TCaptureCity* data = reinterpret_cast<TCaptureCity*>(packet->data());
		data->city_group_id_ = city_group_id;
		data->guild_id_ = guild_id;
		packet->setDataSizeWithoutHeader(sizeof(TCaptureCity));
		return packet;
	}

	inline rnPacket* readyMobServer()
	{
		rnPacket* packet = new rnPacket(0);
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_READY_MOB_SERVER);
		return packet;

	}

	inline rnPacket* resetTotem(tINT instance_id, tINT mob_id)
	{
		rnPacket* packet = new rnPacket(sizeof(TResetTotem));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_RESET_TOTEM);
		TResetTotem* data = reinterpret_cast<TResetTotem*>(packet->data());
		data->instance_id_ = instance_id;
		data->mob_id_ = mob_id;
		packet->setDataSizeWithoutHeader(sizeof(TResetTotem));
		return packet;
	}

	inline rnPacket* updateMaxHp(tINT instance_id, tINT mob_id, tINT max_hp)
	{
		rnPacket* packet = new rnPacket(sizeof(TUpdateMaxHp));
		packet->setGroupType(PGROUP_GAME_ANNOUNCE, PTYPE_GAME_UPDATE_MAX_HP);
		TUpdateMaxHp* data = reinterpret_cast<TUpdateMaxHp*>(packet->data());
		data->instance_id_ = instance_id;
		data->mob_id_ = mob_id;
		data->max_hp_ = max_hp;
		packet->setDataSizeWithoutHeader(sizeof(TUpdateMaxHp));
		return packet;
	}

}
