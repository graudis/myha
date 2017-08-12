#include "stdafx.h"

#include "connmonitorservice_accept.h"
#include "connmonitorservice.h"


MConnMonitorServiceAccept::MConnMonitorServiceAccept()
{
}

MConnMonitorServiceAccept::~MConnMonitorServiceAccept()
{
}

MConnMonitorServiceAccept* MConnMonitorServiceAccept::instance()
{
	static MConnMonitorServiceAccept web_accept;
	return &web_accept;
}

void MConnMonitorServiceAccept::operate(rnSocketIOService* service)
{
	MConnMonitorService* conn_monitor = new MConnMonitorService(service);
	if (conn_monitor == NULL)
	{
		LOG_ERROR("create conn monitor service error");
		service->Close("");
		return;
	}

	service->SetUserData((void*)conn_monitor);

	MConnMonitorManager::instance()->add( service->GetHandle(), conn_monitor );
}
