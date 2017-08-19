#include "stdafx.h"
#include "localrequest.h"
#include "localannounce.h"
#include "localresponse.h"
#include "monitorresponse.h"
#include "monitorannounce.h"

#include "ccenter.h"
#include "myhaSlave.h"

#include "cserver_session.h"

//////////////////////////////////////////////////////////////////////////
// CServerSession
CServerSession::CServerSession(CCenter* center, TServerInfo* server_info) : __center(center)
{
	__server_session_info = *server_info;
	__process_id = 0;

	LOG_INFO("new CServerSession. type: %d - %s, id: %d, channel: %d", getType(), getServerName(getType()), getId(), getChannel());
}

CServerSession::~CServerSession()
{
	LOG_INFO("delete CServerSession. type: %d - %s, id: %d, channel: %d", getType(), getServerName(getType()), getId(), getChannel());
}

bool CServerSession::packetHandler(rnPacket::SP& packet)
{
	return false;
}

void CServerSession::addConnectionInfo(TListenInfo* connection_info)
{
	__listen_info_list.push_back(*connection_info);
}

void CServerSession::sendConnectionInfo(CServerSession* server_session)
{
	std::list<TListenInfo>::iterator it = __listen_info_list.begin();
	std::list<TListenInfo>::iterator end = __listen_info_list.end();

	for (; it != end; it++)
	{
		server_session->sendConnectionInfo(this, &(*it));
	}
}

void CServerSession::sendConnectionInfo(CServerSession* server_session, TListenInfo* listen_info)
{
	// server_session의 listen_info가 this가 접속해야하는 서버라면 아이피,포트를 알려준다.
	if (getType() == listen_info->for_type)
	{
		deliver(CLocalAnnounce::connectionInfo(server_session->getType(), server_session->getId(),
			server_session->getChannel(), listen_info->ip, listen_info->port));
	}
}

void CServerSession::deliver(rnPacket::SP& packet)
{
	__center->deliver(packet);
}

void CServerSession::deliver(rnPacket* packet)
{
	__center->deliver(packet);
}

const char* CServerSession::getIp()
{
	return __center->getIp();
}
