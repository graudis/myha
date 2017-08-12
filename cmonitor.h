#pragma once

#include "localtype.h"

class rnSocketIOService;

class CMonitor : public rnSocketIOHandler
{
public:
	CMonitor();
	~CMonitor();

	void						operate(rnSocketIOService* service);

	rnSocketIOService*			service() { return service_; }

	tBOOL						init(rnSocketIOService* service, tBOOL is_server_process_info = cFALSE);// 모니터 데몬이 비정상 종료후 센터가 강제로 모니터에 접속여부
	void						clear();

	tBOOL						deliver(rnPacket* packet);
	tBOOL						deliver(rnPacket::SP packet);

	void						sendConnectProcessInfo(tINT type, tINT id, tINT channel_id, tINT process_id, const tCHAR* ip);
	void						sendAllUserBan();

	tSINT						getRealtimeEventMapCount() { return realtime_event_map_count_; }
	TRealTimePercent*			getRealTimePercent() { return realtime_event_info_; }

	void						setRealtimeEvent(tBOOL realtime_event_status, tINT map_count, TRealTimePercent* event_info);

private:
	rnSocketIOService*			service_;
	TServerInfo					server_session_info_;

	tBOOL						realtime_event_status_;
	tSINT						realtime_event_map_count_;
	TRealTimePercent*			realtime_event_info_;
};

inline tBOOL CMonitor::deliver(rnPacket* packet)
{
	return deliver(rnPacket::SP(packet));
}

inline tBOOL CMonitor::deliver(rnPacket::SP packet)
{
	if (service_)
	{
		service_->deliver(packet);
		return cTRUE;
	}
	else
	{
		return cFALSE;
	}
}
