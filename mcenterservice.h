#pragma once

class MCenterService : public rnSocketIOHandler
{
private:
	typedef boost::unordered_map< uint32_t, int32_t > map_t;

public:
	MCenterService(rnSocketIOService* service);
	~MCenterService();

	void operate(rnSocketIOService* service);

	void setGroupID(int32_t group_id);
	int32_t getGroupID() { return __group_id; }

	void setProcessID(int32_t process_id) { __process_id = process_id; }
	int32_t getProcessID() { return __process_id; }

	void setloginLimit(int32_t login_limit) { __login_limit = login_limit; }
	int32_t getloginLimit() { return __login_limit; }

	rnSocketIOService* service() { return __service; }

	bool deliver(rnPacket* packet);
	bool deliver(rnPacket::SP packet);

private:
	rnSocketIOService* __service;
	TServerInfo __server_session_info;
	int32_t __group_id;
	int32_t __process_id;
	int32_t __login_limit;
};

inline bool MCenterService::deliver(rnPacket* packet)
{
	if (__service == NULL)
		return false;

	return deliver(rnPacket::SP(packet));
}

inline bool MCenterService::deliver(rnPacket::SP packet)
{
	if (__service == NULL)
		return false;

	__service->deliver(packet);

	return true;
}
