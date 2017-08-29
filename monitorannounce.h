#pragma once

namespace MonitorAnnounce
{
#pragma pack(push, 1)
	struct TProcessStatus
	{
		int32_t	mode;	// 1 : master, 2 : slave
		bool status;	// true : OK, false : Not OK
	};

#pragma pack(pop)

	// function
	inline Packet* ProcessStatus(int32_t mode, bool status)
	{
		Packet* packet = new Packet(sizeof(TProcessStatus));
		packet->setGroupType(PGROUP_MONITOR_ANNOUNCE, PTYPE_MONITOR_Process_Status);
		TProcessStatus* data = reinterpret_cast<TProcessStatus*>(packet->data());
		data->mode = mode;
		data->status = status;
		packet->setDataSizeWithoutHeader(sizeof(TProcessStatus));

		return packet;
	}
}
