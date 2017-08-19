#include "stdafx.h"

#include "localannounce.h"
#include "cmonitor.h"
#include "myhaSlave.h"

#include "cserver_session_manager.h"

CServerSessionManager::CServerSessionManager()
{
}

CServerSessionManager::~CServerSessionManager()
{
}

void CServerSessionManager::addServerSessoin(CServerSession* server_session)
{
	__session_set.insert(server_session);
}

void CServerSessionManager::removeServerSessoin(CServerSession* server_session)
{
	__session_set.erase(server_session);
}

void CServerSessionManager::sendConnectionInfo(CServerSession* server_session)
{
	ServerSessionSet::iterator it = __session_set.begin();
	ServerSessionSet::iterator end = __session_set.end();

	for (; it != end; it++)
	{
		(*it)->sendConnectionInfo(server_session);
	}
}

void CServerSessionManager::sendConnectionInfo(CServerSession* server_session, TListenInfo* listen_info)
{
	ServerSessionSet::iterator it = __session_set.begin();
	ServerSessionSet::iterator end = __session_set.end();

	for (; it != end; it++)
	{
		(*it)->sendConnectionInfo(server_session, listen_info);
	}
}

bool CServerSessionManager::deliver(int32_t type, rnPacket* packet)
{
	rnPacket::SP sp_packet(packet);
	return deliver(type, sp_packet);
}

bool CServerSessionManager::deliver(int32_t type, rnPacket::SP packet)
{
	ServerSessionSet::iterator it = __session_set.begin();
	ServerSessionSet::iterator end = __session_set.end();

	bool is_deliver = false;
	for (; it != end; it++)
	{
		if ((*it)->getType() == type)
		{
			(*it)->deliver(packet);
			is_deliver = true;
		}
	}

	return is_deliver;
}

int32_t CServerSessionManager::getSessionCount(int32_t type)
{
	ServerSessionSet::iterator it = __session_set.begin();
	ServerSessionSet::iterator end = __session_set.end();

	int32_t count = 0;
	for (; it != end; it++)
	{
		if ((*it)->getType() == type)
			count++;
	}

	return count;
}

void CServerSessionManager::sendConnectProcessInfo()
{
}

CServerSession* CServerSessionManager::findServerSession(int32_t type, int32_t map_id, int32_t channel_id)
{
	ServerSessionSet::iterator it = __session_set.begin();
	ServerSessionSet::iterator end = __session_set.end();

	CServerSession* session = NULL;
	for (; it != end; it++)
	{
		if ((*it)->getType() == type)
		{
			if (map_id <= 0)
				return (*it);

			if ((*it)->getId() == map_id)
			{
				if (channel_id < 0)
					return (*it);

				if ((*it)->getChannel() == channel_id)
				{
					return (*it);
				}
			}
		}
	}

	return session;
}
