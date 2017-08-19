#pragma once

#include <map>

class MCenterService;

class MCenterAccept : public rnSocketIOHandler
{
private:
	typedef UNORDERED_MAP<int16_t, MCenterService*>	ip_map_t;

	MCenterAccept();
	~MCenterAccept();

public:
	void operate(rnSocketIOService* service);
	static MCenterAccept* instance();

public:
	MCenterService* lookup(int16_t group_id);
	void createService(int16_t group_id, MCenterService* service);
	void deleteService(MCenterService* service);
	void broadcast(rnPacket* packet);
	void broadcast(rnPacket::SP packet);

private:
	ip_map_t __center_list;
};

