#pragma once

#include <map>
#include "localtype.h"
#include "cserver_session.h"

class rnSocketIOService;

class CCenter : public rnSocketIOHandler
{
public:
	CCenter(rnSocketIOService* service);
	~CCenter();
	void operate(rnSocketIOService* service);

	void handleLocalRequest(rnPacket::SP& packet);
	void handleLocalResponse(rnPacket::SP& packet);

	void deliver(rnPacket::SP& packet);
	void deliver(rnPacket* packet);

	const tCHAR* getIp() { return service_->ip().c_str(); }

private:
	CServerSession* server_session_;

	rnSocketIOService* service_;

};
