#pragma once

#include "cserver_session.h"

class CServerSessionManager
{
public:
	typedef UNORDERED_SET<CServerSession*> ServerSessionSet;

public:
	CServerSessionManager();
	~CServerSessionManager();

	void addServerSessoin(CServerSession* server_session);
	void removeServerSessoin(CServerSession* server_session);

	void sendConnectionInfo(CServerSession* server_session);
	void sendConnectionInfo(CServerSession* server_session, TListenInfo* listen_info);

	bool deliver(int32_t type, rnPacket* packet);
	bool deliver(int32_t type, rnPacket::SP packet);

	int32_t getSessionCount(int32_t type);

	void sendConnectProcessInfo();

	CServerSession* findServerSession(int32_t type, int32_t map_id = 0, int32_t channel_id = -1);

private:
	ServerSessionSet __session_set;
};
