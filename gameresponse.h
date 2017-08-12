#pragma once

namespace GameResponse
{
#pragma pack(push, 1)

	struct GameResponseHeader
	{
		tINT instance_id_;
	};

	// 공격 패킷 응답
	struct TAttackTarget : public GameResponseHeader
	{
		tINT mob_id_;
		tBYTE attack_miss_;
	};

	struct TAttackSelf : public GameResponseHeader
	{
		tBYTE reason_;
		tINT mob_id_;
	};

	struct TPetSummon : public GameResponseHeader
	{
		tINT		skill_id_;
		tINT		mob_id_;
		tINT		script_id_;
		tFLOAT		x_;
		tFLOAT		z_;
		tFLOAT		direction_;
		tINT		pet_number_;
		tBYTE		owner_sort_;
		tINT		owner_id_;
	};

	struct TRescueMob : public GameResponseHeader
	{
		tINT		reason_;
		tINT		user_id_;
		tINT		mob_id_;
	};

	struct TRespawnUniqueMob
	{
		tINT		reason_;
		tINT		guid_;
	};

	struct TActorPetSummon : public GameResponseHeader
	{
		tINT		pet_script_id_;
		tINT		pet_level_script_id_;
		tINT		mob_id_;
		tINT		mob_script_id_;
		tFLOAT		x_;
		tFLOAT		z_;
		tFLOAT		direction_;
		tINT		owner_id_;
	};

	struct TActorPetCommand : public GameResponseHeader
	{
		tBYTE		reason_;
		tINT		owner_id_;
	};

#pragma pack(pop)

	inline rnPacket* castSkill(TAttackTarget& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TAttackTarget));
		packet->setGroupType(PGROUP_GAME_RESPONSE, PTYPE_GAME_CAST_SKILL);
		packet->addVALUE<TAttackTarget>(data);
		return packet;
	}

	inline rnPacket* castSkillSelf(TAttackSelf& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TAttackSelf));
		packet->setGroupType(PGROUP_GAME_RESPONSE, PTYPE_GAME_CAST_SKILL_SELF);
		packet->addVALUE<TAttackSelf>(data);
		return packet;
	}

	inline rnPacket* petSummon(TPetSummon& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TPetSummon));
		packet->setGroupType(PGROUP_GAME_RESPONSE, PTYPE_GAME_PET_SUMMON);
		packet->addVALUE<TPetSummon>(data);
		return packet;
	}

	inline rnPacket* rescueMob(TRescueMob& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TRescueMob));
		packet->setGroupType(PGROUP_GAME_RESPONSE, PTYPE_GAME_RESCUE_MOB);
		packet->addVALUE<TRescueMob>(data);
		return packet;
	}

	inline rnPacket* respawnUniqueMob(tINT reason, tINT guid)
	{
		rnPacket* packet = new rnPacket(sizeof(TRespawnUniqueMob));

		packet->setGroupType(PGROUP_GAME_RESPONSE, PTYPE_GAME_RESPAWN_UNIQUE_MOB);
		TRespawnUniqueMob* data = reinterpret_cast<TRespawnUniqueMob*>(packet->data());
		data->reason_ = reason;
		data->guid_ = guid;
		packet->setDataSizeWithoutHeader(sizeof(TRespawnUniqueMob));

		return packet;
	}

	inline rnPacket* actorPetSummon(TActorPetSummon& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TActorPetSummon));
		packet->setGroupType(PGROUP_GAME_RESPONSE, PTYPE_GAME_ACTOR_PET_SUMMON);
		packet->addVALUE<TActorPetSummon>(data);
		return packet;
	}

	inline rnPacket* actorPetCommand(TActorPetCommand& data)
	{
		rnPacket* packet = new rnPacket(sizeof(TActorPetCommand));
		packet->setGroupType(PGROUP_GAME_RESPONSE, PTYPE_GAME_ACTOR_PET_COMMAND);
		packet->addVALUE<TActorPetCommand>(data);
		return packet;
	}
};
