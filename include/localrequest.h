#pragma once

#include "localtype.h"

namespace CLocalRequest
{
#pragma pack(push, 1)
	struct THaveVIP
	{
	};
#pragma pack(pop)
	inline Packet* haveVIP()
	{
		Packet* packet = new Packet(sizeof(THaveVIP));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_HAVE_VIP);

		return packet;
	}

#pragma pack(push, 1)
	struct TRequestVIPDown
	{
	};
#pragma pack(pop)
	inline Packet* requestVIPDown()
	{
		Packet* packet = new Packet(sizeof(TRequestVIPDown));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_VIP_DOWN);

		return packet;
	}

#pragma pack(push, 1)
	struct TRequestVIPUp
	{
	};
#pragma pack(pop)
	inline Packet* requestVIPUp()
	{
		Packet* packet = new Packet(sizeof(TRequestVIPUp));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_VIP_UP);

		return packet;
	}

	//////////////////////////////////////////////////////////////////////////

	inline Packet* serverInfo(int32_t type, int32_t id, int32_t channel)
	{
		Packet* packet = new Packet(sizeof(TServerInfo));

		packet->setGroupType(PGROUP_LOCAL_REQUEST, PTYPE_LOCAL_SERVER_INFO);
		TServerInfo* data = reinterpret_cast<TServerInfo*>(packet->data());
		data->type = type;
		data->id = id;
		data->channel = channel;
		packet->setDataSizeWithoutHeader(sizeof(TServerInfo));

		return packet;
	}
}
