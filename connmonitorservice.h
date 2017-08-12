#pragma once

#include "localtype.h"
#include "rnpacket.h"

class rnSocketIOService;
class MConnMonitorService;

class MConnMonitorManager
{
private:
	typedef std::map<tINT, MConnMonitorService*> ConnMonitorMap;

private:
	MConnMonitorManager();
	~MConnMonitorManager();

public:
	static MConnMonitorManager* instance();

	void add(tINT handle, MConnMonitorService* service);
	void remove(tINT handle);

	void deliverAll(rnPacket* packet);
	void deliverAll(rnPacket::SP& packet);

private:
	static ConnMonitorMap conn_monitor_map_;

};


class MConnMonitorService : public rnSocketIOHandler
{
	void					operate(rnSocketIOService* service);

public:
	MConnMonitorService(rnSocketIOService* service);
	~MConnMonitorService();

	void					deliver(rnPacket* packet);
	void					deliver(rnPacket::SP packet);

private:
	void					handle_PublicServerList(rnPacket::SP& packet);

private:
	rnSocketIOService*		service_;
};

inline void MConnMonitorService::deliver(rnPacket* packet)
{
	deliver(rnPacket::SP(packet));
}

inline void MConnMonitorService::deliver(rnPacket::SP packet)
{
	if (service_ != NULL)
		service_->deliver(packet);
}
