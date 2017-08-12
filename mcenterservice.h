#pragma once

struct UserCountInfo
{
	tINT crs_user_count_;
	tINT nhn_user_count_;
};


class MCenterService : public rnSocketIOHandler
{
private:
	typedef boost::unordered_map< tUINT, tINT >		map_t;
	typedef boost::unordered_map<tUINT, UserCountInfo>	UserCountInfoMap;

public:
	MCenterService(rnSocketIOService* service);
	~MCenterService();

	void						operate(rnSocketIOService* service);

	void						setGroupID(tINT group_id);
	tINT						getGroupID() { return group_id_; }

	void						setProcessID(tINT process_id) { process_id_ = process_id; }
	tINT						getProcessID() { return process_id_; }

	void						setConnectUsers(tINT crs_count, tINT nhn_count);
	tINT						getCrsConnectUsers() { return crs_connect_users_; }
	tINT						getNhnConnectUsers() { return nhn_connect_users_; }
	tINT						getConnectUsers() { return crs_connect_users_ + nhn_connect_users_; }

	//	void						setMapConnectUsers( tINT map_id, tINT channel_id, tINT crs_user_count, tINT nhn_user_count );
	//	UserCountInfo				getMapConnectUsers();

	void						setloginLimit(tINT login_limit) { login_limit_ = login_limit; }
	tINT						getloginLimit() { return login_limit_; }

	//	tINT						getMapConnectLimit( tINT map_id );
	//	void						setMapConnectLimit( tINT map_id, tINT map_connect_limit );
	//	void						addMapConnectLimit( tINT map_id );

	rnSocketIOService*			service() { return service_; }

	tBOOL						deliver(rnPacket* packet);
	tBOOL						deliver(rnPacket::SP packet);

	tFLOAT						getKillMobExpRate() { return kill_mob_exp_rate_; }

	void setStatusValue(tINT* status_value) { memcpy(status_value_, status_value, sizeof(status_value_)); }
	tINT* getStatusValue() { return status_value_; }

private:
	void handleErrorETIemdOut(rnPacket::SP& packet);


private:
	rnSocketIOService*			service_;
	TServerInfo					server_session_info_;
	tINT						group_id_;
	tINT						process_id_;

	tINT						login_limit_;

	// 그룹별 유저
	tINT						crs_connect_users_;
	tINT						nhn_connect_users_;

	// 맵 채널별 유저
	UserCountInfoMap			map_users_list_;
	//	map_t						map_users_list_;

	map_t						map_user_limit_list_;

	map_t						channel_starting_list_;

	tFLOAT						kill_mob_exp_rate_;
	tINT						status_value_[SERVER_STATUS_VALUE_MAX];
};

inline tBOOL MCenterService::deliver(rnPacket* packet)
{
	if (service_ == NULL)
		return cFALSE;

	return deliver(rnPacket::SP(packet));
}

inline tBOOL MCenterService::deliver(rnPacket::SP packet)
{
	if (service_ == NULL)
		return cFALSE;

	service_->deliver(packet);

	return cTRUE;
}
