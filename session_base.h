#pragma once

#include <boost/version.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

typedef unsigned int session_type;
typedef unsigned int session_handle;

class SessionBase
{
public:
	enum
	{
		INVALID_SESSION_HANDLE = 0
	};

	enum
	{
		SESSION,
		CONNECT_SESSION,
		TIMER_SESSION,
		LISTEN_SESSION,
		EXIT_SESSION
	};

public:
	SessionBase(boost::asio::io_service& io_service, session_type type)	: _io_service(io_service)
	{
		_type = type;
		_handle = INVALID_SESSION_HANDLE;
		_user_data = NULL;
		_close_error = false;
	}
	virtual ~SessionBase() {}

	void SetType(session_type type) { _type = type; }
	session_type GetType() { return _type; }

	void SetHandle(session_handle handle) { _handle = handle; }
	session_handle GetHandle() { return _handle; }

	void SetUserData(void *data) { _user_data = data; }
	void *GetUserData() { return _user_data; }

	void SetErrorMessage(std::string str) { _error_message = str; }
	std::string& GetErrorMessage() { return _error_message; }

	bool isSesseion() { return (_type == SESSION); }
	bool isListenSession() { return (_type == LISTEN_SESSION); }
	bool isTimerSession() { return (_type == TIMER_SESSION); }
	bool isConnectSession() { return (_type == CONNECT_SESSION); }

	virtual bool isValid() { return (_handle != INVALID_SESSION_HANDLE); }

	bool isCloseError() { return _close_error; }

protected:
	session_type _type;
	session_handle _handle;
	void* _user_data;

	std::string _error_message;

	boost::asio::io_service& _io_service;
	bool _close_error;
};
