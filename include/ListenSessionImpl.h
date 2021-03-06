#pragma once

#include <string>

class SocketIOHandler;

class ListenSessionImpl
{
public:
	ListenSessionImpl() {};
	virtual ~ListenSessionImpl() {};

	virtual bool Run(std::string& host, const int port, SocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0) = 0;

	virtual void Close() = 0;

	virtual void closeOnExecOn() = 0;

	virtual void closeOnExecOff() = 0;
};
