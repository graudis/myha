#include "stdafx.h"
#include "instancecooltimemanager.h"
#include "myhaSlave.h"
#include "instancemapmanager.h"
#include "localresponse.h"


//////////////////////////////////////////////////////////////////////////
// InstanceCooltime
InstanceCooltime::InstanceCooltime( tINT user_id ) : user_id_( user_id )
{
	server_session_ = NULL;
	next_send_time_ = 0;
}

InstanceCooltime::~InstanceCooltime()
{
}

tBOOL InstanceCooltime::addCooltime( tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id, tINT cooltime )
{
	tINT instance_key = getInstanceKey( map_id, difficulty );

	CooltimeMap::iterator it = cooltime_map_.find( instance_key );
	if( it == cooltime_map_.end() )
	{
		InstanceInfo instance_info;
		instance_info.map_id_ = map_id;
		instance_info.channel_id_ = channel_id;
		instance_info.difficulty_ = difficulty;
		instance_info.instance_id_ = instance_id;
		instance_info.expired_time_ = myhaSlave::getGlobalTime() + cooltime;

		cooltime_map_.insert( CooltimeMap::value_type( instance_key, instance_info ) );

		LOG_DEBUG( "add cooltime. user_id: %d, map: %d, difficulty: %d, cooltime: %d", user_id_, map_id, difficulty, cooltime );
	}
	else
	{
		if( it->second.expired_time_ > myhaSlave::getGlobalTime() )
			return cFALSE;

		it->second.instance_id_ = instance_id;
		it->second.expired_time_ = myhaSlave::getGlobalTime() + cooltime;

		LOG_DEBUG( "add cooltime. user_id: %d, map: %d, difficulty: %d, cooltime: %d", user_id_, map_id, difficulty, cooltime );
	}

	if( server_session_ != NULL )
	{
		rnPacket* packet = new rnPacket( sizeof(CLocalResponse::TAddInstanceCooltime) );
		packet->setGroupType( PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_ADD_INSTANCE_COOLTIME );

		CLocalResponse::TAddInstanceCooltime* data = (CLocalResponse::TAddInstanceCooltime*)packet->data();
		data->user_id_ = user_id_;

		data->instance_info_.map_id_ = map_id;
		data->instance_info_.channel_id_ = channel_id;
		data->instance_info_.difficulty_ = difficulty;
		data->instance_info_.instance_id_ = instance_id;
		data->instance_info_.cooltime_ = cooltime;
		data->instance_info_.unload_time_ = -1;

		packet->setDataSizeWithoutHeader( sizeof(CLocalResponse::TAddInstanceCooltime) );
		server_session_->deliver( packet );
	}

	return cTRUE;
}

tBOOL InstanceCooltime::hasCooltime( tINT map_id, tINT difficulty )
{
	tINT instance_key = getInstanceKey( map_id, difficulty );

	CooltimeMap::iterator it = cooltime_map_.find( instance_key );
	if( it == cooltime_map_.end() )
		return cFALSE;

	if( it->second.expired_time_ < myhaSlave::getGlobalTime() )
		return cFALSE;

	return cTRUE;
}

tINT InstanceCooltime::getInstanceId( tINT map_id, tINT difficulty )
{
	tINT instance_key = getInstanceKey( map_id, difficulty );

	CooltimeMap::iterator it = cooltime_map_.find( instance_key );
	if( it == cooltime_map_.end() )
		return 0;

	return it->second.instance_id_;
}

tBOOL InstanceCooltime::resetCooltime( tINT map_id, tINT difficulty )
{
	tINT instance_key = getInstanceKey( map_id, difficulty );

	CooltimeMap::iterator it = cooltime_map_.find( instance_key );
	if( it == cooltime_map_.end() )
	{
		// 쿨타임 없음
		return cFALSE;
	}

	LOG_TRACE( "resetCooltime. map_id: %d, difficulty: %d", map_id, difficulty );

	it->second.expired_time_ = myhaSlave::getGlobalTime();
	return cTRUE;
}

void InstanceCooltime::resetCooltimeAll()
{
	CooltimeMap::iterator end = cooltime_map_.end();
	for( CooltimeMap::iterator it = cooltime_map_.begin(); it != end; ++it )
	{
		it->second.expired_time_ = myhaSlave::getGlobalTime();
	}

	sendCooltimeData( NULL );
}

struct TResetInstanceInfo
{
	tINT map_id_;
	tINT difficulty_;
};

void InstanceCooltime::removeMap( CServerSession* server_session )
{
	// 세션 초기화
	if( server_session_ == server_session )
		server_session_ = NULL;

	std::vector<TResetInstanceInfo> reset_vec_;

	// 쿨타임 초기화
	CooltimeMap::iterator it = cooltime_map_.begin();
	CooltimeMap::iterator end = cooltime_map_.end();
	while( it != end )
	{
		CooltimeMap::iterator current = it;
		it++;

		if( current->second.map_id_ == server_session->getId() && current->second.channel_id_ == server_session->getChannel() )
		{
			TResetInstanceInfo reset_instance_info;
			reset_instance_info.map_id_ = current->second.map_id_;
			reset_instance_info.difficulty_ = current->second.difficulty_;
			reset_vec_.push_back( reset_instance_info );

			cooltime_map_.erase( current );
		}
	}

	if( server_session_ != NULL )
	{
		// server_session_이 남아 있다면 리셋된 쿨타임 전송
		rnPacket* packet = new rnPacket( sizeof(CLocalResponse::TResetInstanceList) +
			sizeof(CLocalResponse::TResetInstanceList::TResetInstanceInfo) * reset_vec_.size() );
		packet->setGroupType( PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_RESET_INSTANCE_LIST );

		CLocalResponse::TResetInstanceList* data = (CLocalResponse::TResetInstanceList*)packet->data();
		data->user_id_ = user_id_;
		data->reset_count_ = reset_vec_.size();

		{
			tINT i = 0;
			std::vector<TResetInstanceInfo>::iterator end = reset_vec_.end();
			for( std::vector<TResetInstanceInfo>::iterator it = reset_vec_.begin(); it != end; ++it )
			{
				data->reset_info_[i].map_id_ = it->map_id_;
				data->reset_info_[i].difficulty_ = it->difficulty_;
				i++;
			}
		}

		packet->setDataSizeWithoutHeader( sizeof(CLocalResponse::TResetInstanceList) +
			sizeof(CLocalResponse::TResetInstanceList::TResetInstanceInfo) * reset_vec_.size() );
		server_session_->deliver( packet );
	}
}

void InstanceCooltime::sendCooltimeData( CServerSession* server_session )
{
	if( server_session == NULL )
	{
		if( server_session_ == NULL )
			return;

		server_session = server_session_;
	}

	next_send_time_ = myhaSlave::getGlobalTime() + 60;

//	LOG_TRACE( "InstanceCooltime::sendCooltimeData: %d", cooltime_map_.size() );

	// 보낼 데이터 없음
	if( cooltime_map_.size() == 0 )
		return;

	rnPacket* packet = new rnPacket( sizeof(CLocalResponse::TInstanceCooltimeList) +
		sizeof(TInstanceInfo) * cooltime_map_.size() );
	packet->setGroupType( PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_INSTANCE_COOLTIME_LIST );

	CLocalResponse::TInstanceCooltimeList* data = (CLocalResponse::TInstanceCooltimeList*)packet->data();
	data->user_id_ = user_id_;
	data->instance_count_ = 0;

	tINT i = 0;

	CooltimeMap::iterator it = cooltime_map_.begin();
	CooltimeMap::iterator end = cooltime_map_.end();
	while( it != end )
	{
		CooltimeMap::iterator current = it;
		it++;

		InstanceSave* instance_save = myhaSlave::getInstanceMapManager().findBoundInstanceSave( current->second.map_id_,
			current->second.difficulty_, user_id_, 0 );
		if( instance_save == NULL )
		{
			if( current->second.expired_time_ < myhaSlave::getGlobalTime() )
			{
				cooltime_map_.erase( current );
				continue;
			}
		}

		tINT cooltime = (tINT)(current->second.expired_time_ - myhaSlave::getGlobalTime());
		if( cooltime < 0 )
			cooltime = 0;

		tINT unload_time = 0;
		if( instance_save != NULL )
		{
//			LOG_TRACE( "instance_save_ins: %d, cooltime_ins: %d", instance_save->instance_id_, current->second.instance_id_ );

			if( instance_save->destroy_end_time_ > 0 )
			{
				unload_time = (tINT)(instance_save->destroy_end_time_ - myhaSlave::getGlobalTime());
				if( unload_time < 0 )
					unload_time = 0;
			}
			else
			{
				unload_time = -1;
			}
		}

//		LOG_TRACE( "send data. map_id: %d, diff: %d, ins: %d, unload: %d", current->second.map_id_,
//			current->second.difficulty_, current->second.instance_id_, unload_time );

		data->instance_info_[i].map_id_ = current->second.map_id_;
		data->instance_info_[i].channel_id_ = current->second.channel_id_;
		data->instance_info_[i].difficulty_ = current->second.difficulty_;
		data->instance_info_[i].instance_id_ = current->second.instance_id_;
		data->instance_info_[i].cooltime_ = cooltime;
		data->instance_info_[i].unload_time_ = unload_time;
		i++;

		data->instance_count_++;
	}

	packet->setDataSizeWithoutHeader( sizeof(CLocalResponse::TInstanceCooltimeList) +
		sizeof(TInstanceInfo) * cooltime_map_.size() );
	server_session->deliver( packet );
}

InstanceCooltime::InstanceInfo* InstanceCooltime::getCooltimeInfo( tINT map_id, tINT difficulty )
{
	tINT instance_key = getInstanceKey( map_id, difficulty );

	CooltimeMap::iterator it = cooltime_map_.find( instance_key );
	if( it == cooltime_map_.end() )
	{
		return NULL;
	}

	return &(it->second);
}

tINT InstanceCooltime::getInstanceKey( tINT map_id, tINT difficulty )
{
	return (map_id << 3) | difficulty;
}


//////////////////////////////////////////////////////////////////////////
// InstanceCoolTimeManager
InstanceCoolTimeManager::InstanceCoolTimeManager()
{
}

InstanceCoolTimeManager::~InstanceCoolTimeManager()
{
}

void InstanceCoolTimeManager::update()
{
	tUINT current = myhaSlave::getGlobalTime();

	// 전체 유저 1분마다 인던 쿨타임 내려줌
	InstanceCooltimeMap::iterator end = instance_cooltime_map_.end();
	for( InstanceCooltimeMap::iterator it = instance_cooltime_map_.begin(); it != end; ++it )
	{
		if( it->second->getNextSendTime() > current )
			continue;

		it->second->sendCooltimeData( NULL );
	}
}

tBOOL InstanceCoolTimeManager::addCooltime( tINT user_id, tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id, tINT cooltime )
{
	InstanceCooltimeMap::iterator it = instance_cooltime_map_.find( user_id );

	InstanceCooltime* instance_cooltime = NULL;
	if( it != instance_cooltime_map_.end() )
	{
		instance_cooltime = it->second;
	}
	else
	{
		instance_cooltime = new InstanceCooltime( user_id );
		instance_cooltime_map_.insert( InstanceCooltimeMap::value_type( user_id, instance_cooltime ) );
	}

	return instance_cooltime->addCooltime( map_id, channel_id, difficulty, instance_id, cooltime );
}

void InstanceCoolTimeManager::removeMap( CServerSession* server_session )
{
	InstanceCooltimeMap::iterator end = instance_cooltime_map_.end();
	for( InstanceCooltimeMap::iterator it = instance_cooltime_map_.begin(); it != end; ++it )
	{
		it->second->removeMap( server_session );
	}
}

tBOOL InstanceCoolTimeManager::resetCooltime( tINT user_id , tINT map_id, tINT difficulty )
{
	InstanceCooltimeMap::iterator it = instance_cooltime_map_.find( user_id );
	if( it == instance_cooltime_map_.end() )
	{
		// 쿨타임 없음
		return cFALSE;
	}

	it->second->resetCooltime( map_id, difficulty );

	return cTRUE;
}

void InstanceCoolTimeManager::resetCooltimeAll( tINT user_id )
{
	InstanceCooltimeMap::iterator it = instance_cooltime_map_.find( user_id );
	if( it == instance_cooltime_map_.end() )
		return;

	return it->second->resetCooltimeAll();
}

tBOOL InstanceCoolTimeManager::hasCooltime( tINT user_id, tINT map_id, tINT difficulty )
{
	InstanceCooltimeMap::iterator it = instance_cooltime_map_.find( user_id );
	if( it == instance_cooltime_map_.end() )
		return cFALSE;

	return it->second->hasCooltime( map_id, difficulty );
}

InstanceCooltime::InstanceInfo* InstanceCoolTimeManager::getCooltimeInfo( tINT user_id, tINT map_id, tINT difficulty )
{
	InstanceCooltimeMap::iterator it = instance_cooltime_map_.find( user_id );
	if( it == instance_cooltime_map_.end() )
	{
		return NULL;
	}

	return it->second->getCooltimeInfo( map_id, difficulty );
}

void InstanceCoolTimeManager::sendCooltimeData( tINT user_id, CServerSession* server_session )
{
	InstanceCooltimeMap::iterator it = instance_cooltime_map_.find( user_id );
	if( it == instance_cooltime_map_.end() )
	{
		rnPacket* packet = new rnPacket( sizeof(CLocalResponse::TInstanceCooltimeList) );
		packet->setGroupType( PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_INSTANCE_COOLTIME_LIST );

		CLocalResponse::TInstanceCooltimeList* data = (CLocalResponse::TInstanceCooltimeList*)packet->data();
		data->user_id_ = user_id;
		data->instance_count_ = 0;

		packet->setDataSizeWithoutHeader( sizeof(CLocalResponse::TInstanceCooltimeList) );
		server_session->deliver( packet );
		return;
	}

	it->second->sendCooltimeData( server_session );
}

tBOOL InstanceCoolTimeManager::inMap( tINT user_id, CServerSession* server_session )
{
	InstanceCooltimeMap::iterator it = instance_cooltime_map_.find( user_id );

	InstanceCooltime* instance_cooltime = NULL;
	if( it != instance_cooltime_map_.end() )
	{
		instance_cooltime = it->second;
	}
	else
	{
		instance_cooltime = new InstanceCooltime( user_id );
		instance_cooltime_map_.insert( InstanceCooltimeMap::value_type( user_id, instance_cooltime ) );
	}

	instance_cooltime->setServerSession( server_session );
	return cTRUE;
}

void InstanceCoolTimeManager::outMap( tINT user_id, CServerSession* server_session )
{
	InstanceCooltimeMap::iterator it = instance_cooltime_map_.find( user_id );

	InstanceCooltime* instance_cooltime = NULL;
	if( it != instance_cooltime_map_.end() )
	{
		instance_cooltime = it->second;
	}
	else
	{
		instance_cooltime = new InstanceCooltime( user_id );
		instance_cooltime_map_.insert( InstanceCooltimeMap::value_type( user_id, instance_cooltime ) );
	}

	instance_cooltime->setServerSession( NULL );
}

void InstanceCoolTimeManager::startUnloadTime( tINT map_id, tINT channel_id, tINT instance_id, tINT unload_time )
{
	InstanceSave* instance_save = myhaSlave::getInstanceMapManager().findInstanceSave( map_id, channel_id, instance_id );
	if( instance_save == NULL )
		return;

	instance_save->setDestroyEndTime( myhaSlave::getGlobalTime() + unload_time );

	// 인던에 귀속된 유저들에게 인던 삭제 대기 시간 알려줌
	const InstanceSave::UserList& user_list = instance_save->getUserList();
	InstanceSave::UserList::const_iterator end = user_list.end();
	for( InstanceSave::UserList::const_iterator it = user_list.begin(); it != end; ++it )
	{
		InstanceCooltimeMap::iterator cool_it = instance_cooltime_map_.find( (*it) );
		if( cool_it == instance_cooltime_map_.end() )
			continue;

		if( cool_it->second->getServerSession() == NULL )
			continue;

		sendInstanceUnloadTime( cool_it->second->getServerSession(), cool_it->second->getUserId(), instance_save, unload_time );
	}
}

void InstanceCoolTimeManager::cancelUnloadTime( tINT map_id, tINT channel_id, tINT instance_id )
{
	InstanceSave* instance_save = myhaSlave::getInstanceMapManager().findInstanceSave( map_id, channel_id, instance_id );
	if( instance_save == NULL )
		return;

	instance_save->setDestroyEndTime( -1 );

	const InstanceSave::UserList& user_list = instance_save->getUserList();
	InstanceSave::UserList::const_iterator end = user_list.end();
	for( InstanceSave::UserList::const_iterator it = user_list.begin(); it != end; ++it )
	{
		InstanceCooltimeMap::iterator cool_it = instance_cooltime_map_.find( (*it) );
		if( cool_it == instance_cooltime_map_.end() )
			continue;

		if( cool_it->second->getServerSession() == NULL )
			continue;

		sendInstanceUnloadTime( cool_it->second->getServerSession(), cool_it->second->getUserId(), instance_save, -1 );
	}
}

void InstanceCoolTimeManager::sendInstanceUnloadTime( CServerSession* server_session, tINT user_id, InstanceSave* instance_save, tINT unload_time )
{
	rnPacket* packet = new rnPacket( sizeof(CLocalResponse::TUpdateInstanceUnloadTime) );
	packet->setGroupType( PGROUP_LOCAL_RESPONSE, PTYPE_LOCAL_UPDATE_INSTANCE_UNLOAD_TIME );

	CLocalResponse::TUpdateInstanceUnloadTime* data = (CLocalResponse::TUpdateInstanceUnloadTime*)packet->data();
	data->user_id_ = user_id;

	data->instance_info_.map_id_ = instance_save->map_id_;
	data->instance_info_.channel_id_ = instance_save->channel_id_;
	data->instance_info_.difficulty_ = instance_save->difficulty_;
	data->instance_info_.instance_id_ = instance_save->instance_id_;
	data->instance_info_.cooltime_ = 0;
	data->instance_info_.unload_time_ = unload_time;

	packet->setDataSizeWithoutHeader( sizeof(CLocalResponse::TUpdateInstanceUnloadTime) );
	server_session->deliver( packet );
}
