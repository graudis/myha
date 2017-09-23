#pragma once

#include "type.h"
#include "status.h"
#include "MasterClientAccept.h"

#include "Config_INI.h"

class TimerClass : public SocketIOHandler
{
public:
	TimerClass() {}
	~TimerClass() {}

	void operate(SocketIOService* service);
};

class myhaMaster
{
public:
	myhaMaster();

	static bool init();
	static void end();

	static bool config();
	static bool run();

	static void processTimerSession(TimerSession* session);
	static TimerClass& getTimerClass() { return timer_class; }

public:
	static session_handle server_command_handle;
	static TimerClass timer_class;

public:
	static const char* getServiceIP() { return Config::INI::Instance()->getMasterIP().c_str(); }
	static void requestHaveVIP();
	static void requestVIPDown();
	static void requestVIPUp();
	static void VIPDown();
	static void VIPUp();

public:
	static void sendStatus(bool status);
	static void printState();

public:
	static bool isMySqlOK() { return __mysql_ok; }
	static void setMySqlOK(bool flag) { __mysql_ok = flag; }
private:
	static bool __mysql_ok;

public:
	static bool isConnected() { return __connected; }
	static void setConnected(bool flag) { __connected = flag; }
private:
	static bool __connected;

public:
	static bool isSlaveOK() { return __slave_ok; }
	static void setSlaveOK(bool flag) { __slave_ok = flag; }
private:
	static bool __slave_ok;

public:
	static int32_t HaveVIP();

public:
	static bool haveVIPMaster() { return __have_vip_master; }
	static void setHaveVIPMaster(bool flag) { __have_vip_master = flag; }
private:
	static bool __have_vip_master;

public:
	static bool haveVIPSlave() { return __have_vip_slave; }
	static void setHaveVIPSlave(bool flag) { __have_vip_slave = flag; }
private:
	static bool __have_vip_slave;

private:
	static UStatusChange __status_old;	// master state / slave state
	static UStatusChange __status_cur;
};
