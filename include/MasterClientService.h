#pragma once

class MasterClientService : public SocketIOHandler
{
private:
	typedef boost::unordered_map< uint32_t, int32_t > map_t;

public:
	MasterClientService(SocketIOService* service);
	~MasterClientService();

	void operate(SocketIOService* service);

	void setGroupID(int32_t group_id);
	int32_t getGroupID() { return __group_id; }

	void setProcessID(int32_t process_id) { __process_id = process_id; }
	int32_t getProcessID() { return __process_id; }

	void setloginLimit(int32_t login_limit) { __login_limit = login_limit; }
	int32_t getloginLimit() { return __login_limit; }

	SocketIOService* service() { return __service; }

	bool deliver(Packet* packet);
	bool deliver(Packet::SP packet);

private:
	SocketIOService* __service;
	TServerInfo __server_session_info;
	int32_t __group_id;
	int32_t __process_id;
	int32_t __login_limit;
};
