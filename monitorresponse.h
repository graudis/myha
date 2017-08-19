#pragma once

namespace MonitorResponse
{
#pragma pack(push, 1)
	struct TDaemonStartAlert
	{
		int32_t		group_id;
		uint8_t		sort;
		uint16_t	id;
		uint16_t	channel_id;
		int32_t		process_id;
		char		service_ip[IP_STRING_MAX];
	};
#pragma pack(pop)

	// function
	inline rnPacket* daemonStartAlert(int32_t group_id, uint8_t sort, uint16_t id, uint16_t channel_id, int32_t process_id, const char* service_ip)
	{
		rnPacket* packet = new rnPacket(sizeof(TDaemonStartAlert));

		packet->setGroupType(PGROUP_MONITOR_RESPONSE, PTYPE_MONITOR_DAEMON_START_ALERT);
		TDaemonStartAlert* data = reinterpret_cast<TDaemonStartAlert*>(packet->data());
		data->group_id			= group_id;
		data->sort				= sort;
		data->id				= id;
		data->channel_id		= channel_id;
		data->process_id		= process_id;
		STRNCPY(data->service_ip, service_ip, IP_STRING_MAX);
		packet->setDataSizeWithoutHeader(sizeof(TDaemonStartAlert));

		return packet;
	}
}
