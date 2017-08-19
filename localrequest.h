#pragma once

#include "localtype.h"

namespace CLocalRequest
{
	inline rnPacket* serverInfo(int32_t type, int32_t id, int32_t channel)
	{
		rnPacket* packet = new rnPacket(sizeof(TServerInfo));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_SERVER_INFO);
		TServerInfo* data = reinterpret_cast<TServerInfo*>(packet->data());
		data->type = type;
		data->id = id;
		data->channel = channel;
		packet->setDataSizeWithoutHeader(sizeof(TServerInfo));

		return packet;
	}
}
