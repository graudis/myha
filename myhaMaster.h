#pragma once

#include "type.h"
#include "status.h"
#include "mcenteraccept.h"

#include "rnipdata.h"

//#include "common_config.h"

#include "Config_INI.h"

#define	 IP_LIST_FILE		"../data/ip/ip_list.txt"

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
	static tBOOL				init();
	static void					end();

	static tBOOL				config();
	static tBOOL				run();

//	static tBOOL				loadScripts();

	static void					processTimerSession(TimerSession* PSession);

	static const tCHAR* getPrivateIP() { return ip_data_.getListenIPAddress(cTRUE); }
	static const tCHAR* getPublicIP() { return ip_data_.getListenIPAddress(cFALSE); }

	static TimerClass& getTimerClass() { return timer_class_; }

private:
// 	static void initSignal();
// 	static void signalHandler();

private:
	static rnIpData ip_data_;

public:
// 	static boost_signal_t signal_;
	static session_handle server_command_handle_;
	static TimerClass timer_class_;

public:
	static const tCHAR* getServiceIP() { return Config::INI::Instance()->getMasterIP().c_str(); }
};
