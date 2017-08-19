#include "stdafx.h"

#include "localrequest.h"
#include "localresponse.h"
#include "localannounce.h"
#include "monitorresponse.h"

#include "myhaSlave.h"
#include "cserver_session_manager.h"
#include "ccenter.h"

//////////////////////////////////////////////////////////////////////////
// CCenter
CCenter::CCenter(rnSocketIOService* service) : __service(service)
{
	__server_session = NULL;
}

CCenter::~CCenter()
{
}

void CCenter::operate(rnSocketIOService* service)
{
	rnPacket::SP packet(service->GetMessage());
	if (packet == NULL)
	{
		if (__server_session != NULL)
		{
			myhaSlave::getServerSessionManager().removeServerSessoin(__server_session);
			delete __server_session;
		}

		bnf::instance()->RemoveSession(service->GetHandle());
		delete this;
		return;
	}

	switch (packet->getGroup())
	{
	case PGROUP_LOCAL_RESPONSE:
		handleLocalResponse(packet);
		break;
	}

	if (__server_session != NULL)
		__server_session->packetHandler(packet);
}

void CCenter::handleLocalResponse(rnPacket::SP& packet)
{
	switch (packet->getType())
	{
	case PTYPE_LOCAL_SERVER_INFO:
	{
		if (__server_session != NULL)
		{
			LOG_ERROR("already set info.");
			return;
		}

		TServerInfo* server_info = (TServerInfo*)packet->data();
		switch (server_info->type)
		{
		default:
			__server_session = new CServerSession(this, server_info);
			break;
		}

		// add server
		myhaSlave::getServerSessionManager().addServerSessoin(__server_session);

		// 새로 접속된 서버에게 접속할 서버주소를 보내준다.
		// send connection info
		myhaSlave::getServerSessionManager().sendConnectionInfo(__server_session);
	}
	break;
	case PTYPE_LOCAL_CONNECTION_INFO:
	{
		if (__server_session == NULL)
		{
			LOG_ERROR("unknown server session");
			return;
		}

		TListenInfo* listen_info = (TListenInfo*)packet->data();
		__server_session->addConnectionInfo(listen_info);

		// 새로운 접속 정보를 접속할 서버에게 보내준다.
		myhaSlave::getServerSessionManager().sendConnectionInfo(__server_session, listen_info);
	}
	break;
	}
}

void CCenter::deliver(rnPacket::SP& packet)
{
	if (__service != NULL)
		__service->deliver(packet);
}

void CCenter::deliver(rnPacket* packet)
{
	rnPacket::SP sp_packet(packet);
	deliver(sp_packet);
}
