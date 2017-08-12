#include "stdafx.h"

#include "cserver_session_manager.h"
#include "myhaSlave.h"
#include "cmonitor.h"
#include "localannounce.h"


#define CHANNEL_MAP_RUN_PERCENTAGE			(80)


CServerSessionManager::CServerSessionManager()
{
	total_crs_user_count_ = 0;
	total_nhn_user_count_ = 0;
}

CServerSessionManager::~CServerSessionManager()
{
}

void CServerSessionManager::addServerSessoin(CServerSession* server_session)
{
	session_set_.insert(server_session);
}

void CServerSessionManager::removeServerSessoin(CServerSession* server_session)
{
	session_set_.erase(server_session);
}

void CServerSessionManager::sendConectionInfo(CServerSession* server_session)
{
	ServerSessionSet::iterator it = session_set_.begin();
	ServerSessionSet::iterator end = session_set_.end();

	for (; it != end; it++)
	{
		(*it)->sendConectionInfo(server_session);
	}

	if (server_session->getType() == SERVER_TYPE_GCONTAINER)
	{
		myhaSlave::getBilling().sendConectionInfo(server_session);
		server_session->deliver(CLocalAnnounce::eventFlagSesoTime(myhaSlave::getEventFlagForSesoStartClock(),
			myhaSlave::getEventFlagForSesoEndClock(),
			myhaSlave::isEventFlagForSesoTime(),
			myhaSlave::getEventFlagForSesoPersent()));

		server_session->deliver(CLocalRequest::realtimeEventUpdate(myhaSlave::getMonitor().getRealtimeEventMapCount(), myhaSlave::getMonitor().getRealTimePercent()));
	}
}

void CServerSessionManager::sendConectionInfo(CServerSession* server_session, TListenInfo* listen_info)
{
	ServerSessionSet::iterator it = session_set_.begin();
	ServerSessionSet::iterator end = session_set_.end();

	for (; it != end; it++)
	{
		(*it)->sendConectionInfo(server_session, listen_info);
	}
}

tBOOL CServerSessionManager::deliver(tINT type, rnPacket* packet)
{
	rnPacket::SP sp_packet(packet);
	return deliver(type, sp_packet);
}

tBOOL CServerSessionManager::deliver(tINT type, rnPacket::SP packet)
{
	ServerSessionSet::iterator it = session_set_.begin();
	ServerSessionSet::iterator end = session_set_.end();

	tBOOL is_deliver = cFALSE;
	for (; it != end; it++)
	{
		if ((*it)->getType() == type)
		{
			(*it)->deliver(packet);
			is_deliver = cTRUE;
		}
	}

	return is_deliver;
}

void CServerSessionManager::setTotalCrsUserCount(tINT old_count, tINT new_count)
{
	total_crs_user_count_ -= old_count;
	total_crs_user_count_ += new_count;
}

void CServerSessionManager::setTotalNhnUserCount(tINT old_count, tINT new_count)
{
	total_nhn_user_count_ -= old_count;
	total_nhn_user_count_ += new_count;
}

tINT CServerSessionManager::getSessionCount(tINT type)
{
	ServerSessionSet::iterator it = session_set_.begin();
	ServerSessionSet::iterator end = session_set_.end();

	tINT count = 0;
	for (; it != end; it++)
	{
		if ((*it)->getType() == type)
			count++;
	}

	return count;
}

tBOOL CServerSessionManager::canNextChannleMapRun(tINT map_id, tINT& run_channel_id)
{
	CContainerSession* container = (CContainerSession*)findServerSession(SERVER_TYPE_GCONTAINER, map_id, run_channel_id);
	if (container == NULL)
		return cFALSE;

	while (container != NULL)
	{
		if ((container->getCurrentConnectUsers() / (tFLOAT)container->getMaxUserCount() * 100) < CHANNEL_MAP_RUN_PERCENTAGE)
			return cFALSE;

		run_channel_id++;
		if (run_channel_id >= SERVER_CHANNNEL_ID_MAX)
			return cFALSE;

		container = (CContainerSession*)findServerSession(SERVER_TYPE_GCONTAINER, map_id, run_channel_id);
	}

	return cTRUE;
}

void CServerSessionManager::sendConnectProcessInfo()
{
	ServerSessionSet::iterator it = session_set_.begin();
	ServerSessionSet::iterator end = session_set_.end();

	for (; it != end; it++)
	{
		myhaSlave::getMonitor().sendConnectProcessInfo((*it)->getType(), (*it)->getId(), (*it)->getChannel(), (*it)->getProcessID(), (*it)->getIp());
	}
}

tBOOL CServerSessionManager::isAllMissionMapStatus(tINT map_id, tINT status)
{
	std::pair<ContainerSessionMMap::iterator, ContainerSessionMMap::iterator> ret = container_mmap_.equal_range(map_id);
	for (ContainerSessionMMap::iterator it = ret.first; it != ret.second; ++it)
	{
		CContainerSession* container = it->second;
		if (container->isMissionMapStatus(status) == cFALSE)
			return cFALSE;
	}

	return cTRUE;
}

CContainerSession* CServerSessionManager::getCanJoinMIssioMap(tINT map_id)
{
	std::pair<ContainerSessionMMap::iterator, ContainerSessionMMap::iterator> ret = container_mmap_.equal_range(map_id);
	for (ContainerSessionMMap::iterator it = ret.first; it != ret.second; ++it)
	{
		CContainerSession* container = it->second;

		if (container->canJoinMissionMap() == cTRUE)
			return container;
	}

	return NULL;
}

CContainerSession* CServerSessionManager::getMinUserCountMap(tINT map_id)
{
	tINT min_user_count = 10000000;
	CContainerSession* min_user_continaer = NULL;

	std::pair<ContainerSessionMMap::iterator, ContainerSessionMMap::iterator> ret = container_mmap_.equal_range(map_id);
	for (ContainerSessionMMap::iterator it = ret.first; it != ret.second; ++it)
	{
		CContainerSession* container = it->second;

		if (container->getCurrentConnectUsers() < min_user_count)
		{
			min_user_count = container->getCurrentConnectUsers();
			min_user_continaer = container;
		}
	}

	return min_user_continaer;
}

CServerSession* CServerSessionManager::findServerSession(tINT type, tINT map_id, tINT channel_id)
{
	ServerSessionSet::iterator it = session_set_.begin();
	ServerSessionSet::iterator end = session_set_.end();

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

tBOOL CServerSessionManager::deliverToContainer(tINT map_id, rnPacket* packet)
{
	rnPacket::SP sp_packet(packet);
	return deliverToContainer(map_id, sp_packet);
}

tBOOL CServerSessionManager::deliverToContainer(tINT map_id, rnPacket::SP packet)
{
	std::pair<ContainerSessionMMap::iterator, ContainerSessionMMap::iterator> ret = container_mmap_.equal_range(map_id);
	for (ContainerSessionMMap::iterator it = ret.first; it != ret.second; ++it)
	{
		CContainerSession* container = it->second;
		container->deliver(packet);
	}

	return cTRUE;
}

tBOOL CServerSessionManager::deliverToProxy(tINT proxy_id, rnPacket* packet)
{
	rnPacket::SP sp_packet(packet);
	return deliverToContainer(proxy_id, sp_packet);
}

tBOOL CServerSessionManager::deliverToProxy(tINT proxy_id, rnPacket::SP packet)
{
	ProxySessionMap::iterator it = proxy_map_.find(proxy_id);
	if (it == proxy_map_.end())
		return cFALSE;

	it->second->deliver(packet);

	return cTRUE;
}

void CServerSessionManager::deliverToAllProxy(rnPacket* packet)
{
	ProxySessionMap::iterator it = proxy_map_.begin();
	ProxySessionMap::iterator end = proxy_map_.end();

	rnPacket::SP sp_packet(packet);

	for (; it != end; it++)
	{
		CProxySession* proxy_session = it->second;
		proxy_session->deliver(sp_packet);
	}
}
