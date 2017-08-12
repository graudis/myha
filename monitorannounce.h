#pragma once

namespace MonitorAnnounce
{
#pragma pack(push, 1)

	struct TCurrentConnectUsers
	{
		tINT	total_crs_users_;
		tINT	total_nhn_users_;
	};

	struct TMapConnectUsers
	{
		tINT	map_id_;
		tINT	channel_id_;
		tINT	crs_user_count_;
		tINT	nhn_user_count_;
	};

	struct TServerDown
	{
		tCHAR	message_[80];
	};

	struct TServerUp
	{
		tCHAR	message_[80];
	};

	struct TAbnormalExp
	{
		tINT	actor_id_;
		tCHAR	actor_name_[STATUS_NAME_MAX];
		tINT	old_level_;
		tINT	new_level_;
		exp_t	exp_;
		tINT	log_reason_;
	};

	struct TProcessStatus
	{
		tINT	mode;		// 1 : master, 2 : slave
		tBOOL	status;	// true : OK, false : Not OK
	};

#pragma pack(pop)

	// function
	inline rnPacket* ProcessStatus(tINT mode, tBOOL status)
	{
		rnPacket* packet = new rnPacket(sizeof(TProcessStatus));
		packet->setGroupType(PGROUP_MONITOR_ANNOUNCE, PTYPE_MONITOR_Process_Status);
		TProcessStatus* data = reinterpret_cast<TProcessStatus*>(packet->data());
		data->mode = mode;
		data->status = status;
		packet->setDataSizeWithoutHeader(sizeof(TProcessStatus));

		return packet;
	}

	inline rnPacket* currentConnectUsers( tINT total_crs_users, tINT total_nhn_users )
	{
		rnPacket* packet = new rnPacket( sizeof( TCurrentConnectUsers ) );

		packet->setGroupType( PGROUP_MONITOR_ANNOUNCE, PTYPE_MONITOR_CURRENT_CONNECT_USERS );
		TCurrentConnectUsers* data = reinterpret_cast<TCurrentConnectUsers*>( packet->data() );
		data->total_crs_users_ = total_crs_users;
		data->total_nhn_users_ = total_nhn_users;
		packet->setDataSizeWithoutHeader( sizeof( TCurrentConnectUsers ) );

		return packet;
	}

	inline rnPacket* mapConnectUsers( tINT map_id, tINT channel_id, tINT crs_user_count, tINT nhn_user_count )
	{
		rnPacket* packet = new rnPacket( sizeof( TMapConnectUsers ) );

		packet->setGroupType( PGROUP_MONITOR_ANNOUNCE, PTYPE_MONITOR_MAP_CONNECT_USERS );
		TMapConnectUsers* data = reinterpret_cast<TMapConnectUsers*>( packet->data() );
		data->map_id_		= map_id;
		data->channel_id_	= channel_id;
		data->crs_user_count_ = crs_user_count;
		data->nhn_user_count_ = nhn_user_count;
		packet->setDataSizeWithoutHeader( sizeof( TMapConnectUsers ) );

		return packet;
	}

	inline rnPacket* serverDown( const tCHAR* message )
	{
		rnPacket* packet = new rnPacket( sizeof( TServerDown ) );
		packet->setGroupType( PGROUP_MONITOR_ANNOUNCE, PTYPE_MONITOR_SERVER_DOWN );
		TServerDown* data = reinterpret_cast<TServerDown*>( packet->data() );
		STRNCPY( data->message_, message, sizeof(data->message_) );
		packet->setDataSizeWithoutHeader( sizeof( TServerDown ) );
		return packet;
	}

	inline rnPacket* serverUp( const tCHAR* message )
	{
		rnPacket* packet = new rnPacket( sizeof( TServerUp ) );
		packet->setGroupType( PGROUP_MONITOR_ANNOUNCE, PTYPE_MONITOR_SERVER_UP );
		TServerUp* data = reinterpret_cast<TServerUp*>( packet->data() );
		STRNCPY( data->message_, message, sizeof(data->message_) );
		packet->setDataSizeWithoutHeader( sizeof( TServerUp ) );
		return packet;
	}

	inline rnPacket* abnormalExp( tINT actor_id, const tCHAR* actor_name, tINT old_level, tINT new_level, exp_t exp, tINT log_reason )
	{
		rnPacket* packet = new rnPacket( sizeof(TAbnormalExp) );
		packet->setGroupType( PGROUP_MONITOR_ANNOUNCE, PTYPE_MONITOR_ABNORMAL_EXP );
		TAbnormalExp* data = reinterpret_cast<TAbnormalExp*>( packet->data() );
		data->actor_id_ = actor_id;
		STRNCPY( data->actor_name_, actor_name, sizeof(data->actor_name_) );
		data->old_level_ = old_level;
		data->new_level_ = new_level;
		data->exp_ = exp;
		data->log_reason_ = log_reason;
		packet->setDataSizeWithoutHeader( sizeof(TAbnormalExp) );
		return packet;
	}
}
