#pragma once

#include <stdlib.h>
#include <string.h>
#include <string>

#include "type.h"
#include "boostcommon.h"

//////////////////////////////////////////////////////////////////////////
// actor와 관련없는 define
#define MAKE_MAP_CHANNEL_ID(x, y)					((x) << 16 | (y))

union UStatusChange
{
	uint32_t status;
	struct
	{
		uint32_t master : 1;
		uint32_t slave : 1;
	} data;

	void reset() { status = 0; }
	void statusMaster(bool flag) { data.master = (flag > 0) ? 1 : 0; }
	void statusSlave(bool flag) { data.slave = (flag > 0) ? 1 : 0; }
};
