#include "stdafx.h"

#include "ccenter_accept.h"

#include "myhaSlave.h"

#include "ccenter.h"

CCenterAccept::CCenterAccept()
{
}

CCenterAccept::~CCenterAccept()
{
}

void CCenterAccept::operate(rnSocketIOService* service)
{
	CCenter* center = new CCenter(service);
	service->SetUserData(center);

	service->deliver(CLocalRequest::serverInfo(SERVER_TYPE_CENTER, myhaSlave::getGroupID(), 0));
}

