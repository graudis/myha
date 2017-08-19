#pragma once

#include "type.h"
#include "status.h"
#include "mcenteraccept.h"

#include "Config_INI.h"

class TimerClass : public rnSocketIOHandler
{
public:
	TimerClass() {}
	~TimerClass() {}

	void operate(rnSocketIOService* service);
};

class myhaMaster
{
public:
	static bool init();
	static void end();

	static bool config();
	static bool run();

	static void processTimerSession(TimerSession* PSession);

	static TimerClass& getTimerClass() { return timer_class; }

public:
	static session_handle server_command_handle;
	static TimerClass timer_class;

public:
	static const char* getServiceIP() { return Config::INI::Instance()->getMasterIP().c_str(); }
};
