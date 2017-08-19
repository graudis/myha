#pragma once

class rnSocketIOService;

class rnSocketIOHandler
{
public:
	rnSocketIOHandler();
	virtual ~rnSocketIOHandler();

	virtual void operate(rnSocketIOService* service) = 0;

	virtual void onConnect(rnSocketIOService* service) {}
	virtual void onConnectFail(rnSocketIOService* service) {}

	virtual void onEvent( void* data ) {}
};
