#include "stdafx.h"

#include "connmonitorservice.h"

#include "myhaMaster.h"


MConnMonitorManager::ConnMonitorMap MConnMonitorManager::conn_monitor_map_;


MConnMonitorManager::MConnMonitorManager()
{
}

MConnMonitorManager::~MConnMonitorManager()
{
}

MConnMonitorManager* MConnMonitorManager::instance()
{
	static MConnMonitorManager instance;
	return &instance;
}

void MConnMonitorManager::add( tINT handle, MConnMonitorService* service )
{
	conn_monitor_map_.insert( ConnMonitorMap::value_type( handle, service ) );
}

void MConnMonitorManager::remove( tINT handle )
{
	conn_monitor_map_.erase( handle );
}

void MConnMonitorManager::deliverAll( rnPacket* packet )
{
	rnPacket::SP packet_sp( packet );
	deliverAll( packet_sp );
}

void MConnMonitorManager::deliverAll( rnPacket::SP& packet )
{
	ConnMonitorMap::iterator end = conn_monitor_map_.end();

	for( ConnMonitorMap::iterator it = conn_monitor_map_.begin(); it != end; ++it )
	{
		MConnMonitorService* service = it->second;
		service->deliver( packet );
	}
}


MConnMonitorService::MConnMonitorService(rnSocketIOService* service)
{
	service_ = service;
}

MConnMonitorService::~MConnMonitorService()
{
	service_ = NULL;
}

void MConnMonitorService::operate(rnSocketIOService* service)
{
	rnPacket::SP packet(service->GetMessage());
	if (packet == NULL)
	{
		LOG_DEBUG("[%s:%d] reason : %s", __PRETTY_FUNCTION__, __LINE__, service->GetErrorMessage().c_str());
		bnf::instance()->RemoveSession(service->GetHandle());
		MConnMonitorManager::instance()->remove(service->GetHandle());
		delete this;
		return;
	}

}
