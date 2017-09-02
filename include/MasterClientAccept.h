#pragma once

#include <map>

class MasterClientService;

class MasterClientAccept : public SocketIOHandler
{
private:
	typedef UNORDERED_MAP<int16_t, MasterClientService*>	ip_map_t;

	MasterClientAccept();
	~MasterClientAccept();

public:
	void operate(SocketIOService* service);
	static MasterClientAccept* instance();

public:
	MasterClientService* lookup(int16_t group_id);
	void createService(int16_t group_id, MasterClientService* service);
	void deleteService(MasterClientService* service);
	void broadcast(Packet* packet);
	void broadcast(Packet::SP packet);

private:
	ip_map_t __center_list;
};
