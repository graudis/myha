#pragma once

#include "localtype.h"

class rnSocketIOService;
class CServerSession;

class CBilling : public rnSocketIOHandler
{
public:
	CBilling();
	~CBilling();

	void						operate(rnSocketIOService* service);

	rnSocketIOService*			service() { return service_; }

	tBOOL						init(rnSocketIOService* service);
	void						clear();
	TListenInfo*				getListenInfo() { return &listen_info_; }


	tBOOL						deliver(rnPacket* packet);
	tBOOL						deliver(rnPacket::SP packet);

	void						sendConnectProcessInfo(tINT type, tINT id, tINT channel_id, tINT process_id, const tCHAR* ip);
	void						sendConectionInfo();
	void						sendConectionInfo(CServerSession* server_session);
	void						sendGlogConnectInfo();

private:
	rnSocketIOService*			service_;
	TServerInfo					server_session_info_;
	TListenInfo					listen_info_;
};

inline tBOOL CBilling::deliver(rnPacket* packet)
{
	return deliver(rnPacket::SP(packet));
}

inline tBOOL CBilling::deliver(rnPacket::SP packet)
{
	if (service_)
	{
		service_->deliver(packet);
		return cTRUE;
	}
	else
	{
		return cFALSE;
	}
}
