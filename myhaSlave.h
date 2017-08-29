#pragma once

#include "ccenter.h"
#include "localrequest.h"

#include "MasterClient.h"
#include "cserver_session_manager.h"

#define CONNECTED_SERVER_GLOG		0x0001
#define CONNECTED_SERVER_ALL		(CONNECTED_SERVER_GLOG)
#define INIT_SERVER_COMPLETED		0x1000

#define EVENT_FLAG_SESO_TIME		30

class TimerClass2 : public SocketIOHandler
{
public:
	TimerClass2() {}
	~TimerClass2() {}

	void operate(SocketIOService* service);
};

class myhaSlave
{
public:
	static bool init(int32_t groupid);

	static bool run();
	static bool loadScripts();

	static void Stop();

	static bool endService();

	static int32_t getGroupID() { return __group_id; }

	static void processTimerSession(TimerSession* pSession);

	static CServerSessionManager& getServerSessionManager() { return __server_session_manager; }
	static MasterClient& getMonitor() { return __master_client; }
	static TimerClass2& getTimerClass() { return timer_class; }

	static time_t getGlobalTime() { return __global_time; }

	static void setConnectFlag(int32_t flag);
	static bool isInitCompleted() { return (__connect_flag & INIT_SERVER_COMPLETED) == INIT_SERVER_COMPLETED; }

	static bool isServerInit() { return __server_init; }

public:
	static session_handle connect_monitor_handle;
	static session_handle update_timer;

	static TimerClass2 timer_class;

private:
	static bool initService();

	static void initSignal();
	static void signalHandler();

	static int32_t __group_id;

	static CServerSessionManager __server_session_manager;
	static MasterClient __master_client;

	static time_t __global_time;
	static int32_t __connect_flag;

private:
	static bool __server_init;
};
