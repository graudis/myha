#pragma once

#include "ccenter.h"
#include "localrequest.h"

#include "rnipdata.h"
#include "ccenter_accept.h"
#include "cmonitor.h"
#include "cbilling.h"
#include "cserver_session_manager.h"
#include "instancemapmanager.h"
#include "instancecooltimemanager.h"

#include "cdbworker.h"

#define	 IP_LIST_FILE		"../data/ip/ip_list.txt"

#define CONNECTED_SERVER_GLOG		0x0001
#define CONNECTED_SERVER_ALL		(CONNECTED_SERVER_GLOG)
#define INIT_SERVER_COMPLETED		0x1000

#define EVENT_FLAG_SESO_TIME		30

extern RnadomNumberGenerator sRandom;

class TimerClass2 : public rnSocketIOHandler
{
public:
	TimerClass2() {}
	~TimerClass2() {}

	void operate(rnSocketIOService* service);
};

class myhaSlave
{
public:
	static tBOOL init(tINT groupid);

	static tBOOL						run();
	static tBOOL						loadScripts();

	static void							Stop();

	static tBOOL						endService();

	static tINT							groupid() { return groupid_; }

	// 	static const tCHAR*					login_ip()					{ return common_config_.getLoginIp(); }
	// 	static const tCHAR*					monitor_ip()				{ return common_config_.getMonitorIp(); }
	// 	static const tCHAR*					billing_ip()				{ return common_config_.getBillingIp(); }
	// 	static const tCHAR* getServiceIP() { return ip_data()->getListenIPAddress(common_config_.usePrivateNetwork()); }
	// 	static tINT							default_login_limit()		{ return common_config_.getDefaultLoginLimit(groupid()); }
	// 	static tINT							proxy_max()					{ return common_config_.getProxyClientMax(); }
	// 	static tTIME						getCurrentConnectUserTime()	{ return common_config_.getCurrentConnectUserTime();	}

	static rnIpData*					ip_data() { return &ip_data_; };

	static void							processTimerSession(TimerSession* pSession);


	static tINT							getusers_count_max() { return users_count_max_; }
	static void							setusers_count_max(tINT users_count_max) { users_count_max_ = users_count_max; }

	static tINT							getcrs_users_count_max() { return crs_users_count_max_; }
	static void							setcrs_users_count_max(tINT crs_users_count_max) { crs_users_count_max_ = crs_users_count_max; }

	static tINT							getnhn_users_count_max() { return nhn_users_count_max_; }
	static void							setnhn_users_count_max(tINT nhn_users_count_max) { nhn_users_count_max_ = nhn_users_count_max; }

	static CServerSessionManager&		getServerSessionManager() { return server_session_manager_; }
	// 	static CLogin&						getLogin() { return login_; }
	static CCenterAccept&				getCenterAccept() { return center_accept_; }
	static CMonitor&					getMonitor() { return monitor_; }
	static CBilling&					getBilling() { return billing_; }
	static TimerClass2&					getTimerClass() { return timer_class_; }

	static InstanceMapManager&			getInstanceMapManager() { return instance_map_manager_; }

	// 	static MobServerManager&			getMobServerManager() { return mob_server_manager_; }

	static tTIME						getGlobalTime() { return global_time_; }

	static void							setConnectFlag(tINT flag);
	static tBOOL						isInitCompleted() { return (connect_flag_ & INIT_SERVER_COMPLETED) == INIT_SERVER_COMPLETED; }

	static tBOOL						isEventFlagForSesoTime() { return event_flag_for_sesotime_; }
	static tINT							getEventFlagForSesoPersent() { return event_flag_for_seso_persent_; }
	static tINT							getEventFlagForSesoStartClock() { return event_flag_for_sesotime_start_clock_; }
	static tINT							getEventFlagForSesoEndClock() { return event_flag_for_sesotime_end_clock_; }

	static InstanceCoolTimeManager&		getInstanceCoolTimeManager() { return instance_cooltime_manager_; }
	// 	static tBOOL isBoostExpServer() { return common_config_.isBoostExpServer(groupid()); }

	static void							setServerInit();
	static tBOOL						isServerInit() { return server_init_; }

public:
	static session_handle				connect_login_handle_;
	static session_handle				connect_monitor_handle_;
	static session_handle				service_expire_time_handle_;
	static session_handle				connect_billing_handle_;
	static session_handle				update_timer_;
	static session_handle				chage_day_handle_;
	static session_handle				stop_roulette_handle_;
	static session_handle				event_seso_time_handle_;

	// event
	static tBOOL						event_flag_for_sesotime_;
	static tINT							event_flag_for_seso_persent_;
	static tINT							event_flag_for_sesotime_start_clock_;
	static tINT							event_flag_for_sesotime_end_clock_;

	static TimerClass2					timer_class_;

private:
	static tBOOL						initService();

	static void							initSignal();
	static void							signalHandler();

	static tINT							groupid_;

	static session_handle				current_connect_count_handle_;

	static rnIpData						ip_data_;
	static tINT							users_count_max_;
	static tINT							crs_users_count_max_;
	static tINT							nhn_users_count_max_;

	// 	static CommonConfig					common_config_;

	static CServerSessionManager		server_session_manager_;
	// 	static CLogin						login_;
	static CCenterAccept				center_accept_;
	static CMonitor						monitor_;
	static CBilling						billing_;

	static InstanceMapManager			instance_map_manager_;
	// 	static MobServerManager				mob_server_manager_;

	static tTIME						global_time_;
	static tINT							connect_flag_;
	static InstanceCoolTimeManager		instance_cooltime_manager_;

public:
	static TDatabaseInfo				gamedb_info_;
	// 	static CDBWorker&					getDBWorker()			{ return dbWorker_; }
	// 	static tBOOL						endCenterMobServer();

private:
	// 	static CDBWorker					dbWorker_;

	static tBOOL						server_init_;
};
