#pragma once

#include "localtype.h"

class rnSocketIOService;

class CMonitor : public rnSocketIOHandler
{
public:
	CMonitor();
	~CMonitor();

	void operate(rnSocketIOService* service);

	rnSocketIOService* service() { return __service; }

	bool init(rnSocketIOService* service, bool is_server_process_info = false);// 모니터 데몬이 비정상 종료후 센터가 강제로 모니터에 접속여부
	void clear();

	bool deliver(rnPacket* packet);
	bool deliver(rnPacket::SP packet);

private:
	rnSocketIOService* __service;
	TServerInfo __server_session_info;
};

inline bool CMonitor::deliver(rnPacket* packet)
{
	return deliver(rnPacket::SP(packet));
}

inline bool CMonitor::deliver(rnPacket::SP packet)
{
	if (__service != NULL)
	{
		__service->deliver(packet);
		return true;
	}

	return false;
}
