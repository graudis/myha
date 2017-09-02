#pragma once

#include <map>
#include "localtype.h"
#include "cserver_session.h"

class SocketIOService;

class CCenter : public SocketIOHandler
{
public:
	CCenter(SocketIOService* service);
	~CCenter();
	void operate(SocketIOService* service);

// 	void handleLocalRequest(Packet::SP& packet);
	void handleLocalResponse(Packet::SP& packet);

	void deliver(Packet::SP& packet);
	void deliver(Packet* packet);

	const char* getIp() { return __service->ip().c_str(); }

private:
	CServerSession* __server_session;
	SocketIOService* __service;
};
