#pragma once

#include "dodb_type.h"

namespace ChatAnnounce
{
#pragma pack(push, 1)
	struct TChatGmPrivateNotify
	{
		tCHAR		actor_name_[STATUS_NAME_MAX];
		tUINT		time_;
		tBYTE		length_;
		tCHAR		msg_[0];
	};

	struct TChatGmPublicNotify
	{
		tUINT		time_;
		tBYTE		length_;
		tCHAR		msg_[0];
	};

	struct TChatPrivateNotify
	{
		tCHAR		actor_name_[STATUS_NAME_MAX];
		tUINT		time_;
		tBYTE		length_;
		tCHAR		msg_[0];
	};

	struct TChatSystemNotify
	{
		tUINT		time_;
		tBYTE		length_;
		tCHAR		msg_[0];
	};

	struct TChatGmMapNotify
	{
		tINT		map_id_;
		tUINT		time_;
		tBYTE		length_;
		tCHAR		msg_[0];
	};
#pragma pack(pop)

	// function
	inline rnPacket* chatGmPrivateNotify(tUINT time, const tCHAR* actor_name, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatGmPrivateNotify));

		packet->setGroupType(PGROUP_CHAT_ANNOUNCE, PTYPE_CHAT_GM_PRIVATE_NOTIFY);
		TChatGmPrivateNotify* data = reinterpret_cast<TChatGmPrivateNotify*>(packet->data());
		STRNCPY(data->actor_name_, actor_name, sizeof(data->actor_name_));
		data->time_ = time;
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatGmPrivateNotify));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}

	inline rnPacket* chatGmPublicNotify(tUINT time, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatGmPublicNotify));

		packet->setGroupType(PGROUP_CHAT_ANNOUNCE, PTYPE_CHAT_GM_PUBLIC_NOTIFY);
		TChatGmPublicNotify* data = reinterpret_cast<TChatGmPublicNotify*>(packet->data());
		data->time_ = time;
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatGmPublicNotify));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}

	inline rnPacket* chatPrivateNotify(tUINT time, const tCHAR* actor_name, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatPrivateNotify));

		packet->setGroupType(PGROUP_CHAT_ANNOUNCE, PTYPE_CHAT_PRIVATE_NOTIFY);
		TChatPrivateNotify* data = reinterpret_cast<TChatPrivateNotify*>(packet->data());
		STRNCPY(data->actor_name_, actor_name, sizeof(data->actor_name_));
		data->time_ = time;
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatPrivateNotify));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}

	inline rnPacket* chatSystemNotify(tUINT time, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatSystemNotify));

		packet->setGroupType(PGROUP_CHAT_ANNOUNCE, PTYPE_CHAT_SYSTEM_NOTIFY);
		TChatSystemNotify* data = reinterpret_cast<TChatSystemNotify*>(packet->data());
		data->time_ = time;
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatSystemNotify));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}

	inline rnPacket* chatGmMapNotify(tINT map_id, tUINT time, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatGmMapNotify));

		packet->setGroupType(PGROUP_CHAT_ANNOUNCE, PTYPE_CHAT_GM_MAP_NOTIFY);
		TChatGmMapNotify* data = reinterpret_cast<TChatGmMapNotify*>(packet->data());
		data->map_id_ = map_id;
		data->time_ = time;
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatGmMapNotify));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}
};
