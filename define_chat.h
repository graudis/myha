#pragma once

#include "rntype.h"

#define CHAT_TITLE_NAME_MAX				(14 + 1)			// 체널 제목 길이
#define CHAT_MESSAGE_MAX				(150 + 1)			// 채팅 내용 길이
#define CHAT_CUSTOM_CHANNEL_MAX			(3)					// 커스텀 채널 최대 갯수
#define CHAT_PARTY_SEARCH_LEVEL_MIN		10					// 파티 채팅 가능 레벨
#define CHAT_GUILD_SHORT_NOTIFY			(80 + 1)			// 길드 공지(작은것)
//#define CHAT_MESSAGE_SPAM_TIME			7					// 스팸 방지 시간(초)

enum {
	CHAT_ERR_NOT_ERROR,						// 정상
	CHAT_ERR_CHANNEL_NAME_MAX,				// 체널 이름이 최대치 초과
	CHAT_ERR_CHANNEL_CREATE_MAX,			// 최대 체널 생성치 초과
	CHAT_ERR_CHANNEL_FIND_FAIL,				// 체널을 찾을수 없습니다.
	CHAT_ERR_CHANNEL_CREATE_FAIL,			// 체널 생성 실패
	CHAT_ERR_USER_FIND_FAIL,				// 유저 동기화 에러
	CHAT_ERR_CHANNEL_NOT_MASTER,			// 챗장만의 권한입니다.
	CHAT_ERR_CHANNEL_BAN_USER,				// 강퇴 당한 체널
	CHAT_ERR_CHANNEL_JOIN_FAIL,				// 입장 실패
	CHAT_ERR_CHANNEL_KICK,					// 추방 유저 없음
	CHAT_ERR_CHANNEL_BAN,					// 영구추방 유저 없음
	CHAT_ERR_CHANNEL_ENTRUST,				// 영구추방 유저 없음
	CHAT_ERR_CHANNEL_CREATE_DUPLICATION,	// 이미 같은 이름의 채널이 있습니다.
	CHAT_ERR_WHISPER_NOT_FIND_TARGET_ACTOR,	// 전음을 전달할 상대가 없음
	CHAT_ERR_WHISPER_TARGET_REJECT,			// 타겟이 귓말 거부중
};

struct TChatActorStatus
{
	tINT		actor_id_;
	tCHAR		actor_name_[STATUS_NAME_MAX];
	tINT		map_id_;
	guild_id_t	guild_id_;
	tINT		party_id_;
};

namespace ChatMember
{
#pragma pack(push, 1)
	// 타겟이 없는 채팅
	struct TChatOnly
	{
		tBYTE		length_;
		tCHAR		msg_[0];
	};

	// 타겟이 없는 채팅 아이템 첨부형
	struct TChatOnlyAppendItem
	{
		TOwnItemBase	item_;
		tBYTE			length_;
		tCHAR			msg_[0];
	};

	// 특정 타겟에 채팅
	struct TChatActor
	{
		tCHAR		actor_name_[STATUS_NAME_MAX];
		tBYTE		length_;
		tCHAR		msg_[0];
	};

	// 특정 타겟에 채팅 아이템 첨부형
	struct TChatActorAppendSeq
	{
		tINT			seq_key_;
		tCHAR			actor_name_[STATUS_NAME_MAX];
		tBYTE			length_;
		tCHAR			msg_[0];
	};

	struct TChatActorAppendItem
	{
		TOwnItemBase	item_;
		tCHAR			actor_name_[STATUS_NAME_MAX];
		tBYTE			length_;
		tCHAR			msg_[0];
	};

	// 개인 채널 채팅
	struct TChatChannel
	{
		tINT		channel_id_;
		tBYTE		length_;
		tCHAR		msg_[0];
	};

	// 개인 채널 채팅 아이템 첨부형
	struct TChatChannelAppendItem
	{
		TOwnItemBase	item_;
		tINT			channel_id_;
		tBYTE			length_;
		tCHAR			msg_[0];
	};

	// 개인 채널 타겟 채팅
	struct TChatChannelActor
	{
		tCHAR		actor_name_[STATUS_NAME_MAX];
		tINT		channel_id_;
		tBYTE		length_;
		tCHAR		message_[0];
	};

	// 개인 채널 타겟 채팅 아이템 첨부형
	struct TChatChannelActorAppendSeq
	{
		tINT			seq_key_;
		tCHAR			actor_name_[STATUS_NAME_MAX];
		tINT			channel_id_;
		tBYTE			length_;
		tCHAR			message_[0];
	};

#pragma pack(pop)

	inline rnPacket* chatOnly(const tINT group_id, const tINT type_id, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatOnly) + length);

		packet->setGroupType(group_id, type_id);
		TChatOnly* data = reinterpret_cast<TChatOnly*>(packet->data());
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatOnly));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}

	inline rnPacket* chatOnly(const tINT group_id, const tINT type_id, const TOwnItemBase* item, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatOnlyAppendItem) + length);

		packet->setGroupType(group_id, type_id);
		TChatOnlyAppendItem* data = reinterpret_cast<TChatOnlyAppendItem*>(packet->data());
		::memcpy(&data->item_, item, sizeof(TOwnItemBase));
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatOnlyAppendItem));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}


	inline rnPacket* chatActor(const tINT group_id, const tINT type_id, const tCHAR* actor_name, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatActor) + length);

		packet->setGroupType(group_id, type_id);
		TChatActor* data = reinterpret_cast<TChatActor*>(packet->data());
		STRNCPY(data->actor_name_, actor_name, sizeof(data->actor_name_));
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatActor));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}


	inline rnPacket* chatActor(const tINT group_id, const tINT type_id, tINT seq_key, const tCHAR* actor_name, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatActorAppendSeq) + length);

		packet->setGroupType(group_id, type_id);
		TChatActorAppendSeq* data = reinterpret_cast<TChatActorAppendSeq*>(packet->data());
		data->seq_key_ = seq_key;
		STRNCPY(data->actor_name_, actor_name, sizeof(data->actor_name_));
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatActorAppendSeq));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}

	inline rnPacket* chatChannel(const tINT group_id, const tINT type_id, tINT channel_id, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatChannel) + length);

		packet->setGroupType(group_id, type_id);
		TChatChannel* data = reinterpret_cast<TChatChannel*>(packet->data());
		data->channel_id_ = channel_id;
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatChannel));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}

	inline rnPacket* chatChannel(const tINT group_id, const tINT type_id, const TOwnItemBase* item, tINT channel_id, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatChannelAppendItem) + length);

		packet->setGroupType(group_id, type_id);
		TChatChannelAppendItem* data = reinterpret_cast<TChatChannelAppendItem*>(packet->data());
		::memcpy(&data->item_, item, sizeof(TOwnItemBase));
		data->channel_id_ = channel_id;
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatChannelAppendItem));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}


	inline rnPacket* chatChannelActor(const tINT group_id, const tINT type_id, const tCHAR* actor_name, tINT channel_id, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatChannelActor) + length);

		packet->setGroupType(group_id, type_id);
		TChatChannelActor* data = reinterpret_cast<TChatChannelActor*>(packet->data());
		STRNCPY(data->actor_name_, actor_name, sizeof(data->actor_name_));
		data->channel_id_ = channel_id;
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatChannelActor));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}

	inline rnPacket* chatChannelActor(const tINT group_id, const tINT type_id, tINT seq_key, const tCHAR* actor_name, tINT channel_id, const tCHAR* message, tBYTE length)
	{
		rnPacket* packet = new rnPacket(sizeof(TChatChannelActorAppendSeq) + length);

		packet->setGroupType(group_id, type_id);
		TChatChannelActorAppendSeq* data = reinterpret_cast<TChatChannelActorAppendSeq*>(packet->data());
		data->seq_key_ = seq_key;
		STRNCPY(data->actor_name_, actor_name, sizeof(data->actor_name_));
		data->channel_id_ = channel_id;
		data->length_ = length;
		packet->setDataSizeWithoutHeader(sizeof(TChatChannelActorAppendSeq));
		packet->addVALUE((tBYTE*)message, length);

		return packet;
	}
};
