#pragma once

#include "localtype.h"

namespace CLocalAnnounce
{
	inline Packet* connectionInfo(int32_t for_type, int32_t id, int32_t channel, char* ip, int32_t port)
	{
		Packet* packet = new Packet(sizeof(TConnectionInfo));

		packet->setGroupType(PGROUP_LOCAL_ANNOUNCE, PTYPE_LOCAL_CONNECTION_INFO);
		TConnectionInfo* data = reinterpret_cast<TConnectionInfo*>(packet->data());
		data->type = for_type;
		data->id = id;
		data->channel_id = channel;
		STRNCPY(data->ip, ip, sizeof(data->ip));
		data->port = port;
		packet->setDataSizeWithoutHeader(sizeof(TConnectionInfo));

		return packet;
	}
}
