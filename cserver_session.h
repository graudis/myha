#pragma once

#include "localtype.h"
#include "define_missionmap.h"

class CCenter;

class CServerSession
{
public:
	CServerSession(CCenter* center, TServerInfo* server_info);
	virtual ~CServerSession();

	virtual tBOOL packetHandler(rnPacket::SP& packet);

	void addConnectionInfo(TListenInfo* connection_info);

	void sendConectionInfo(CServerSession* server_session);
	virtual void sendConectionInfo(CServerSession* server_session, TListenInfo* listen_info);

	TServerInfo* getServerSessionInfo() { return &server_session_info_; }
	tINT getType() { return server_session_info_.type_; }
	tINT getId() { return server_session_info_.id_; }
	tINT getChannel() { return server_session_info_.channel_; }

	void deliver(rnPacket::SP& packet);
	void deliver(rnPacket* packet);

	const tCHAR* getIp();

	void setProcessID(tINT process_id) { process_id_ = process_id; };
	tINT getProcessID() { return process_id_; };

protected:
	CCenter* center_;
	TServerInfo server_session_info_;
	tINT process_id_;

	std::list<TListenInfo> listen_info_list_;
};

class CProxySession : public CServerSession
{
public:
	CProxySession(CCenter* center, TServerInfo* server_info);
	~CProxySession();

	tBOOL packetHandler(rnPacket::SP& packet);

	tINT getUserCount() { return crs_user_count_ + nhn_user_count_; }
	void sendChangeDay();

private:
	tINT crs_user_count_;
	tINT nhn_user_count_;
};


class CHcashSession : public CServerSession
{
public:
	CHcashSession(CCenter* center, TServerInfo* server_info);
	~CHcashSession();

	tBOOL packetHandler(rnPacket::SP& packet);
};

class CContainerSession : public CServerSession
{
public:
	typedef UNORDERED_SET<tINT> QueuePlayersSet;

public:
	CContainerSession(CCenter* center, TServerInfo* server_info);
	~CContainerSession();

	tBOOL packetHandler(rnPacket::SP& packet);
	void sendConectionInfo(CServerSession* server_session, TListenInfo* listen_info);

	void setChannelMapInfo(tBOOL is_channel_map, tINT max_user_count);
	void setCurrentConnectUsers(tINT current_connect_users) { current_connect_users_ = current_connect_users; }

	tBOOL isChannelMap() { return is_channel_map_; }
	tINT getMaxUserCount() { return max_user_count_; }
	tINT getCurrentConnectUsers() { return current_connect_users_; }

	tBOOL isMissionMapStatus(tINT status) { return mission_map_info_.status_ == status; }

	void addMissionMapWaitQueue(tINT actor_id) { wait_actor_set_.insert(actor_id); }

	tBOOL canJoinMissionMap()
	{
		if (isMissionMapStatus(MISSIONMAP_STATUS_OPEN) == cFALSE)
			return cFALSE;

		if (mission_map_info_.max_players_ <= getCurrentConnectUsers() + (tINT)wait_actor_set_.size())
			return cFALSE;

		return cTRUE;
	}

private:
	void handleLocalOpenMissionMap(rnPacket::SP& packet);
	void handleLocalCloseMissionMap(rnPacket::SP& packet);

	void handleLocalJoinMissionMapReq(rnPacket::SP& packet);
	void handleLocalJoinMissionMapRes(rnPacket::SP& packet);
	void handleLocalRemoveQueueMissionMap(rnPacket::SP& packet);
	void handleLocalMissionMapAnnounce(rnPacket::SP& packet);

	void handleCreateInstance(rnPacket::SP& packet);
	void handleDestroyInstance(rnPacket::SP& packet);
	void handleBindInstance(rnPacket::SP& packet);
	void handleUnBindUserInstance(rnPacket::SP& packet);
	void handleUnBindPartyInstance(rnPacket::SP& packet);
	void handleQueryBoundInstance(rnPacket::SP& packet);
	void handleResetInstance(rnPacket::SP& packet);
	void handleResetInstanceAll(rnPacket::SP& packet);
	void handleInstanceCooltimeList(rnPacket::SP& packet);
	void handleResetInstanceCooltimeAll(rnPacket::SP& packet);
	void handleStartDestroyInstance(rnPacket::SP& packet);
	void handleCancelDestroyInstance(rnPacket::SP& packet);
	void handleCenterInMap(rnPacket::SP& packet);
	void handleCenterOutMap(rnPacket::SP& packet);
	void handleRaidMObRegenStatus(rnPacket::SP& packet);

private:
	tINT current_connect_users_;
	tBOOL is_channel_map_;
	tINT max_user_count_;

	struct MissionMapInfo
	{
		tINT map_id_;
		tINT status_;
		tINT min_level_;
		tINT max_level_;
		tINT max_players_;
	};

	MissionMapInfo mission_map_info_;
	QueuePlayersSet wait_actor_set_;
};

class CMobSession : public CServerSession
{
public:
	CMobSession(CCenter* center, TServerInfo* server_info);
	~CMobSession();

	tBOOL packetHandler(rnPacket::SP& packet);
	void sendConectionInfo(CServerSession* server_session, TListenInfo* listen_info);

private:
	void handleGameResponse_RespawnUniqueMob(rnPacket::SP& packet);
	void handleGameAnnounce_DeadUniqueMob(rnPacket::SP& packet);
	void handleGameAnnounce_ReadyMobServer(rnPacket::SP& packet);
};

class CDodbSession : public CServerSession
{
public:
	CDodbSession(CCenter* center, TServerInfo* server_info);
	~CDodbSession();

	tBOOL packetHandler(rnPacket::SP& packet);
};

class CChatSession : public CServerSession
{
public:
	CChatSession(CCenter* center, TServerInfo* server_info);
	~CChatSession();

	tBOOL packetHandler(rnPacket::SP& packet);

	tCHAR* getServiceIp() { return ip_; }
	tINT getServicePort() { return port_; }

	tCHAR ip_[IP_STRING_MAX];
	tINT port_;
};
