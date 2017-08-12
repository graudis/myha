#pragma once

class CServerSession;
class InstanceSave;

class InstanceCooltime
{
public:
	struct InstanceInfo
	{
		tINT map_id_;
		tINT channel_id_;
		tINT difficulty_;
		tINT instance_id_;
		tUINT expired_time_;
	};

	typedef std::map<tINT, InstanceInfo> CooltimeMap; // (map_id << 3) | difficulty, InstanceInfo

public:
	InstanceCooltime(tINT user_id);
	~InstanceCooltime();

	tINT getUserId() { return user_id_; }

	tBOOL addCooltime(tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id, tINT cooltime);
	tBOOL hasCooltime(tINT map_id, tINT difficulty);
	tINT getInstanceId(tINT map_id, tINT difficulty);
	void removeMap(CServerSession* server_session);

	tBOOL resetCooltime(tINT map_id, tINT difficulty);
	void resetCooltimeAll();

	void sendCooltimeData(CServerSession* server_session);

	InstanceInfo* getCooltimeInfo(tINT map_id, tINT difficulty);

	void setServerSession(CServerSession* server_session) { server_session_ = server_session; }
	CServerSession* getServerSession() { return server_session_; }

	tUINT getNextSendTime() { return next_send_time_; }

public:
	static tINT getInstanceKey(tINT map_id, tINT difficulty);

private:
	tINT user_id_;
	CooltimeMap cooltime_map_;
	CServerSession* server_session_;

	tUINT next_send_time_;
};


class InstanceCoolTimeManager
{
private:
	typedef std::map<tINT, InstanceCooltime*> InstanceCooltimeMap; // user_id, InstanceCooltime*

public:
	InstanceCoolTimeManager();
	~InstanceCoolTimeManager();

	void update();

	tBOOL addCooltime(tINT user_id, tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id, tINT cooltime);
	void removeMap(CServerSession* server_session);

	tBOOL resetCooltime(tINT user_id, tINT map_id, tINT difficulty);
	void resetCooltimeAll(tINT user_id);

	tBOOL hasCooltime(tINT user_id, tINT map_id, tINT difficulty);
	InstanceCooltime::InstanceInfo* getCooltimeInfo(tINT user_id, tINT map_id, tINT difficulty);

	void sendCooltimeData(tINT user_id, CServerSession* server_session);

	tBOOL inMap(tINT user_id, CServerSession* server_session);
	void outMap(tINT user_id, CServerSession* server_session);

	void startUnloadTime(tINT map_id, tINT channel_id, tINT instance_id, tINT unload_time);
	void cancelUnloadTime(tINT map_id, tINT channel_id, tINT instance_id);

private:
	void sendInstanceUnloadTime(CServerSession* server_session, tINT user_id, InstanceSave* instance_save, tINT unload_time);

private:
	InstanceCooltimeMap instance_cooltime_map_;

};
