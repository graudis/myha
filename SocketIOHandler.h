#pragma once

class SocketIOService;

class SocketIOHandler
{
public:
	SocketIOHandler();
	virtual ~SocketIOHandler();

	virtual void operate(SocketIOService* service) = 0;

	virtual void onConnect(SocketIOService* service) {}
	virtual void onConnectFail(SocketIOService* service) {}

	virtual void onEvent( void* data ) {}
};
