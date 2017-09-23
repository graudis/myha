#pragma once

#include "localtype.h"

namespace CLocalResponse
{
	inline Packet* serverInfo(int32_t type, int32_t id, int32_t channel)
	{
		Packet* packet = new Packet(sizeof(TServerInfo));

		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_SERVER_INFO);
		TServerInfo* data = reinterpret_cast<TServerInfo*>(packet->data());
		data->type = type;
		data->id = id;
		data->channel = channel;
		packet->setDataSizeWithoutHeader(sizeof(TServerInfo));

		return packet;
	}

#pragma pack(push, 1)
	struct THaveVIP
	{
		bool have_vip;
	};
#pragma pack(pop)
	inline Packet* hasVIP(bool has_vip)
	{
		Packet* packet = new Packet(sizeof(THaveVIP));

		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_HAVE_VIP);
		THaveVIP* data = reinterpret_cast<THaveVIP*>(packet->data());
		data->have_vip = has_vip;
		packet->setDataSizeWithoutHeader(sizeof(THaveVIP));

		return packet;
	}

	inline Packet* listenInfo(int32_t for_type, const char* ip, int32_t port)
	{
		Packet* packet = new Packet(sizeof(TListenInfo));

		packet->setGroupType(PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_CONNECTION_INFO);
		TListenInfo* data = reinterpret_cast<TListenInfo*>(packet->data());
		data->for_type = for_type;
		STRNCPY(data->ip, ip, sizeof(data->ip));
		data->port = port;
		packet->setDataSizeWithoutHeader(sizeof(TListenInfo));

		return packet;
	}
}
