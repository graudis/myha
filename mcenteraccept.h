#pragma once

#include <map>

class MCenterService;

class MCenterAccept : public rnSocketIOHandler
{
private:
	typedef UNORDERED_MAP<tSINT, MCenterService*>	ip_map_t;

	MCenterAccept();
	~MCenterAccept();

public:
	void								operate(rnSocketIOService* service);
	static MCenterAccept*				instance();

public:
	MCenterService*						lookup(tSINT group_id);
	void								createService(tSINT group_id, MCenterService* service);
	void								deleteService(MCenterService* service);
	void								broadcast(rnPacket* packet);
	void								broadcast(rnPacket::SP packet);

	void								setCurrentConnectUser(tINT current_crs_count, tINT current_nhn_count);
	tINT								getCurrentCrsCount() { return current_crs_count_; }
	tINT								getCurrentNhnCount() { return current_nhn_count_; }
	tINT								getCurrentConnectUser() { return current_crs_count_ + current_nhn_count_; }

	void								sendAllConnectInfo(rnSocketIOService* service);

private:
	ip_map_t							center_list_;

	tINT								current_crs_count_;
	tINT								current_nhn_count_;
};

