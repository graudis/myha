#pragma once

#include "type.h"
#include "status.h"

enum PLOCAL_TYPE_
{
	PTYPE_LOCAL_NONE = 0,
	PTYPE_LOCAL_SERVER_INFO,
	PTYPE_LOCAL_CONNECTION_INFO,
	PTYPE_LOCAL_HAVE_VIP,
	PTYPE_LOCAL_VIP_DOWN,
	PTYPE_LOCAL_VIP_UP,
	PTYPE_LOCAL_MAX,
};

struct TConnectionInfo
{
	uint8_t		type;
	uint16_t	id;
	uint16_t	channel_id;
	char		ip[IP_STRING_MAX];
	int32_t		port;
};

struct TServerInfo
{
	int32_t type;
	int16_t id;
	int32_t channel;
};

struct TListenInfo
{
	uint8_t	for_type;
	char	ip[IP_STRING_MAX];
	int32_t	port;
};

inline const char* getServerName(int32_t sort)
{
	switch (sort)
	{
	case SERVER_TYPE_NONE:		return "None";		break;
	case SERVER_TYPE_SLAVE:	return "Center";	break;
	case SERVER_TYPE_MASTER:	return "Monitor";	break;
	}

	return "Unknown";
}
