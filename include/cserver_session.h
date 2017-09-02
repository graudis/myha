#pragma once

#include "localtype.h"

class CCenter;

class CServerSession
{
public:
	CServerSession(CCenter* center, TServerInfo* server_info);
	virtual ~CServerSession();

	virtual bool packetHandler(Packet::SP& packet);

	void addConnectionInfo(TListenInfo* connection_info);

	void sendConnectionInfo(CServerSession* server_session);
	virtual void sendConnectionInfo(CServerSession* server_session, TListenInfo* listen_info);

	TServerInfo* getServerSessionInfo() { return &__server_session_info; }
	int32_t getType() { return __server_session_info.type; }
	int32_t getId() { return __server_session_info.id; }
	int32_t getChannel() { return __server_session_info.channel; }

	void deliver(Packet::SP& packet);
	void deliver(Packet* packet);

	const char* getIp();

	void setProcessID(int32_t process_id) { __process_id = process_id; };
	int32_t getProcessID() { return __process_id; };

protected:
	CCenter* __center;
	TServerInfo __server_session_info;
	int32_t __process_id;

	std::list<TListenInfo> __listen_info_list;
};
