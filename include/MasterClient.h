#pragma once

#include "localtype.h"

class SocketIOService;

class MasterClient : public SocketIOHandler
{
public:
	MasterClient();
	~MasterClient();

	void operate(SocketIOService* service);

	SocketIOService* service() { return __service; }

	bool init(SocketIOService* service, bool is_server_process_info = false);// 모니터 데몬이 비정상 종료후 센터가 강제로 모니터에 접속여부
	void clear();

	bool deliver(Packet* packet);
	bool deliver(Packet::SP packet);

private:
	SocketIOService* __service;
	TServerInfo __server_session_info;
};

inline bool MasterClient::deliver(Packet* packet)
{
	return deliver(Packet::SP(packet));
}

inline bool MasterClient::deliver(Packet::SP packet)
{
	if (__service != NULL)
	{
		__service->deliver(packet);
		return true;
	}

	return false;
}
