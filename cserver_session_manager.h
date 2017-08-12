#pragma once

#include "cserver_session.h"

class CServerSessionManager
{
public:
	typedef UNORDERED_SET<CServerSession*>				ServerSessionSet;
	typedef UNORDERED_MAP<tINT, CProxySession*>			ProxySessionMap;
	typedef UNORDERED_MAP<tINT, CChatSession*>			ChatSessionMap;
	typedef std::multimap<tINT, CContainerSession*>		ContainerSessionMMap;

public:
	CServerSessionManager();
	~CServerSessionManager();

	void addServerSessoin(CServerSession* server_session);
	void removeServerSessoin(CServerSession* server_session);

	void sendConectionInfo(CServerSession* server_session);
	void sendConectionInfo(CServerSession* server_session, TListenInfo* listen_info);

	// 	void sendProxyListToLogin( CLogin* login );
	// 	void sendChatListToLogin( CLogin* login );

	tBOOL deliver(tINT type, rnPacket* packet);
	tBOOL deliver(tINT type, rnPacket::SP packet);

	void setTotalCrsUserCount(tINT old_count, tINT new_count);
	void setTotalNhnUserCount(tINT old_count, tINT new_count);

	tINT getTotalCrsUserCount() { return total_crs_user_count_; }
	tINT getTotalNhnUserCount() { return total_nhn_user_count_; }

	tINT getTotalUserCount() { return total_crs_user_count_ + total_nhn_user_count_; }
	tINT getSessionCount(tINT type);

	tBOOL canNextChannleMapRun(tINT map_id, tINT& run_channel_id);

	void sendConnectProcessInfo();

	tBOOL isAllMissionMapStatus(tINT map_id, tINT status);
	CContainerSession* getCanJoinMIssioMap(tINT map_id);
	CContainerSession* getMinUserCountMap(tINT map_id);

	CServerSession* findServerSession(tINT type, tINT map_id = 0, tINT channel_id = -1);

	tBOOL deliverToContainer(tINT map_id, rnPacket* packet);
	tBOOL deliverToContainer(tINT map_id, rnPacket::SP packet);

	tBOOL deliverToProxy(tINT proxy_id, rnPacket* packet);
	tBOOL deliverToProxy(tINT proxy_id, rnPacket::SP packet);
	void deliverToAllProxy(rnPacket* packet);

private:
	ServerSessionSet session_set_;
	ProxySessionMap proxy_map_;
	ChatSessionMap chat_map_;

	ContainerSessionMMap container_mmap_;

	tINT total_crs_user_count_;
	tINT total_nhn_user_count_;
};
