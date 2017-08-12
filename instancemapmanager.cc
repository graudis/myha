#include "stdafx.h"

#include "instancemapmanager.h"
#include "instancecooltimemanager.h"
#include "myhaSlave.h"

//////////////////////////////////////////////////////////////////////////
// InstanceSave
InstanceSave::InstanceSave( tINT map_id, tINT channel_id, tINT instance_id, tINT difficulty )
{
	map_id_ = map_id;
	channel_id_ = channel_id;
	instance_id_ = instance_id;
	difficulty_ = difficulty;
	destroy_end_time_ = -1;
}

InstanceSave::~InstanceSave()
{
}

//////////////////////////////////////////////////////////////////////////
// InstanceUserBind
void InstanceUserBind::bindInstance( tINT map_id, InstanceSave* instance_save )
{
	boundInstances_.insert( BoundInstancesMap::value_type( map_id, instance_save ) );
	instance_save->addUser( user_id_ );
}

void InstanceUserBind::unbindInstance( tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id )
{
	// map_id, channel_id, instance_id 이 인던에서 귀속 해제 시킴

	std::pair<BoundInstancesMap::iterator, BoundInstancesMap::iterator> ret = boundInstances_.equal_range( map_id );
	for( BoundInstancesMap::iterator it = ret.first; it != ret.second; ++it )
	{
		InstanceSave* instance_save = it->second;
		if( instance_save->channel_id_ == channel_id && instance_save->difficulty_ == difficulty &&
			instance_save->instance_id_ == instance_id )
		{
			instance_save->removeUser( user_id_ );

			LOG_INFO( "InstanceMapManager. unbind user. user: %d, map: %d, channel: %d, instance: %d",
				user_id_, map_id, channel_id, instance_save->instance_id_ );
			boundInstances_.erase( it );
			break;
		}
	}
}

void InstanceUserBind::unbindInstanceAll()
{
	BoundInstancesMap::iterator end = boundInstances_.end();
	for( BoundInstancesMap::iterator it = boundInstances_.begin(); it != end; ++it )
	{
		InstanceSave* instance_save = it->second;

		instance_save->removeUser( user_id_ );

		LOG_INFO( "InstanceMapManager. unbind user. user: %d, map: %d, channel: %d, instance: %d",
			user_id_, instance_save->map_id_, instance_save->channel_id_, instance_save->instance_id_ );
	}

	boundInstances_.clear();
}

InstanceSave* InstanceUserBind::getBoundInstance( tINT map_id, tINT difficulty )
{
	std::pair<BoundInstancesMap::iterator, BoundInstancesMap::iterator> ret = boundInstances_.equal_range( map_id );
	for( BoundInstancesMap::iterator it = ret.first; it != ret.second; ++it )
	{
		if( it->second->difficulty_ == difficulty )
			return it->second;
	}

	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// InstancePartyBind
void InstancePartyBind::bindInstance( tINT map_id, InstanceSave* instance_save )
{
	boundInstances_.insert( BoundInstancesMap::value_type( map_id, instance_save ) );
	instance_save->addParty( party_id_ );
}

void InstancePartyBind::unbindInstance( tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id )
{
	// map_id, channel_id, instance_id 이 인던에서 귀속 해제 시킴
	std::pair<BoundInstancesMap::iterator, BoundInstancesMap::iterator> ret = boundInstances_.equal_range( map_id );
	for( BoundInstancesMap::iterator it = ret.first; it != ret.second; ++it )
	{
		InstanceSave* instance_save = it->second;
		if( instance_save->channel_id_ == channel_id && instance_save->difficulty_ == difficulty &&
			instance_save->instance_id_ == instance_id )
		{
			instance_save->removeParty( party_id_ );

			LOG_INFO( "InstanceMapManager. unbind party. party: %d, map: %d, channel: %d, instance: %d",
				party_id_, map_id, channel_id, instance_save->instance_id_ );
			boundInstances_.erase( it );
			break;
		}
	}
}

InstanceSave* InstancePartyBind::getBoundInstance( tINT map_id, tINT difficulty )
{
	std::pair<BoundInstancesMap::iterator, BoundInstancesMap::iterator> ret = boundInstances_.equal_range( map_id );
	for( BoundInstancesMap::iterator it = ret.first; it != ret.second; ++it )
	{
		if( it->second->difficulty_ == difficulty )
			return it->second;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// InstanceMapManager
InstanceMapManager::InstanceMapManager()
{
}

InstanceMapManager::~InstanceMapManager()
{
}

tBOOL InstanceMapManager::createInstance( tINT map_id, tINT channel_id, tINT instance_id, tINT difficulty )
{
	InstanceSave* instance_save = findInstanceSave( map_id, channel_id, instance_id );
	if( instance_save != NULL )
	{
		LOG_ERROR( "InstanceMapManager. already created. map: %d, channel: %d, instance: %d",
			map_id, channel_id, instance_id );
		return cFALSE;
	}

	// 인던 객체 생성
	instance_save = new InstanceSave( map_id, channel_id, instance_id, difficulty );

	ContainerMap* container_map = NULL;

	std::pair<ContainerMapList::iterator, ContainerMapList::iterator> ret = map_list_.equal_range( map_id );
	for( ContainerMapList::iterator it = ret.first; it != ret.second; ++it )
	{
		if( it->second->getChannelId() == channel_id )
		{
			container_map = it->second;
		}
	}

	if( container_map == NULL )
	{
		container_map = new ContainerMap( map_id, channel_id );
		map_list_.insert( ContainerMapList::value_type( map_id, container_map ) );

		LOG_INFO( "InstanceMapManager. add map. map: %d, channel: %d", map_id, channel_id );
	}

	container_map->addInstance( instance_save );
	LOG_INFO( "InstanceMapManager. create. map: %d, channel: %d, instance: %d, count: %d",
		map_id, channel_id, instance_id, container_map->getInstanceCount() );
	return cTRUE;
}

void InstanceMapManager::destroyInstance( tINT map_id, tINT channel_id, tINT instance_id )
{
	InstanceSave* instance_save = findInstanceSave( map_id, channel_id, instance_id );
	if( instance_save == NULL )
	{
		LOG_ERROR( "InstanceMapManager. not found. map: %d, channel: %d, instance: %d",
			map_id, channel_id, instance_id );
		return;
	}

	// instance_save에 귀속되 있는 유저, 파티 정리
	clearInstanceSave( instance_save );

	tINT instance_count = 0;
	std::pair<ContainerMapList::iterator, ContainerMapList::iterator> ret = map_list_.equal_range( map_id );
	for( ContainerMapList::iterator it = ret.first; it != ret.second; ++it )
	{
		if( it->second->getChannelId() == channel_id )
		{
			ContainerMap* container_map = it->second;
			container_map->removeInstance( instance_id );

			instance_count = container_map->getInstanceCount();
			break;
		}
	}

	// 인던 삭제
	delete instance_save;

	LOG_INFO( "InstanceMapManager. destroy. map: %d, channel: %d, instance: %d, count: %d",
		map_id, channel_id, instance_id, instance_count );
}

InstanceSave* InstanceMapManager::bindInstance( tINT map_id, tINT channel_id, tINT instance_id, tINT user_id, tINT party_id, tBOOL* user_bind )
{
	// 인던에 귀속 시킨다
	InstanceSave* instance_save = findInstanceSave( map_id, channel_id, instance_id );
	if( instance_save == NULL )
	{
		LOG_ERROR( "InstanceMapManager. not found. map: %d, channel: %d, instance: %d",
			map_id, channel_id, instance_id );
		return NULL;
	}

	if( instance_save->hasUser( user_id ) == cFALSE )
	{
		// 유저 아이디로 검색. 개인이 사용중인 인던 얻어옴
		InstanceUserBind* instance_user_bind = NULL;
		{
			InstanceUserBindMap::iterator it = user_bind_map_.find( user_id );
			if( it != user_bind_map_.end() )
				instance_user_bind = it->second;
		}

		if( instance_user_bind == NULL )
		{
			instance_user_bind = new InstanceUserBind( user_id );
			user_bind_map_.insert( InstanceUserBindMap::value_type( user_id, instance_user_bind ) );
		}

		// 유저가 사용중인 인던에 새로운 인던 추가
		instance_user_bind->bindInstance( map_id, instance_save );

		LOG_INFO( "InstanceMapManager. bind user. user_id: %d map: %d, channel: %d, instance: %d",
			user_id, map_id, channel_id, instance_id );

		*user_bind = cTRUE;
	}

	if( party_id > 0 )
	{
		if( instance_save->hasParty( party_id ) == cFALSE )
		{
			// 파티 아이디로 검색. 파티원들이 사용중인 인던 얻어옴
			InstancePartyBind* instance_party_bind = NULL;
			{
				InstancePartyBindMap::iterator it = party_bind_map_.find( party_id );
				if( it != party_bind_map_.end() )
					instance_party_bind = it->second;
			}

			if( instance_party_bind == NULL )
			{
				instance_party_bind = new InstancePartyBind( party_id );
				party_bind_map_.insert( InstancePartyBindMap::value_type( party_id, instance_party_bind ) );
			}

			// 파티원이 사용중인 인던에 새로운 인던 추가
			instance_party_bind->bindInstance( map_id, instance_save );

			LOG_INFO( "InstanceMapManager. bind party. party: %d, map: %d, channel: %d, instance: %d",
				party_id, map_id, channel_id, instance_id );
		}
	}

	return instance_save;
}

void InstanceMapManager::unbindInstanceUser( tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id, tINT user_id )
{
	// 유저가 인던에서 나와서 인던 귀속 해제
	InstanceUserBind* instance_user_bind = NULL;
	{
		InstanceUserBindMap::iterator it = user_bind_map_.find( user_id );
		if( it != user_bind_map_.end() )
			instance_user_bind = it->second;
	}

	if( instance_user_bind != NULL )
	{
		// map_id, channel_id 안에 귀속된 인던 삭제
		instance_user_bind->unbindInstance( map_id, channel_id, difficulty, instance_id );
		if( instance_user_bind->boundInstances_.empty() == true )
		{
			// 유저가 귀속된 인던이 하나도 없다면 유저가 사용중인 인던 리스트 관리할 필요 없음
			user_bind_map_.erase( user_id );
			delete instance_user_bind;
		}
	}
}

void InstanceMapManager::unbindInstanceParty( tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id, tINT party_id )
{
	// 파티가 인던에서 나와서 인던 귀속 해제
	InstancePartyBind* instance_party_bind = NULL;
	{
		InstancePartyBindMap::iterator it = party_bind_map_.find( party_id );
		if( it != party_bind_map_.end() )
			instance_party_bind = it->second;
	}

	if( instance_party_bind != NULL )
	{
		// map_id, channel_id 안에 귀속된 인던 삭제
		instance_party_bind->unbindInstance( map_id, channel_id, difficulty, instance_id );
		if( instance_party_bind->boundInstances_.empty() == true )
		{
			// 파티가 귀속된 인던이 하나도 없다면 파티가 사용중인 인던 리스트 관리할 필요 없음
			party_bind_map_.erase( party_id );
			delete instance_party_bind;
		}
	}
}

void InstanceMapManager::unbindInstanceAll( tINT user_id )
{
	InstanceUserBind* instance_user_bind = NULL;
	{
		InstanceUserBindMap::iterator it = user_bind_map_.find( user_id );
		if( it != user_bind_map_.end() )
			instance_user_bind = it->second;
	}

	if( instance_user_bind != NULL )
	{
		instance_user_bind->unbindInstanceAll();
		if( instance_user_bind->boundInstances_.empty() == true )
		{
			// 유저가 귀속된 인던이 하나도 없다면 유저가 사용중인 인던 리스트 관리할 필요 없음
			user_bind_map_.erase( user_id );
			delete instance_user_bind;
		}
	}
}

InstanceSave* InstanceMapManager::findBoundInstanceSave( tINT map_id, tINT difficulty, tINT user_id, tINT party_id, tBOOL* party_bind )
{
	// 개인 귀속 부터 체크
	InstanceUserBind* instance_user_bind = NULL;
	{
		InstanceUserBindMap::iterator it = user_bind_map_.find( user_id );
		if( it != user_bind_map_.end() )
			instance_user_bind = it->second;
	}

	if( instance_user_bind != NULL )
	{
		InstanceSave* instance_save = instance_user_bind->getBoundInstance( map_id, difficulty );
		if( instance_save != NULL )
			return instance_save;
	}

	// 파티 귀속 체크
	if( party_id > 0 )
	{
		InstancePartyBind* instance_party_bind = NULL;
		{
			InstancePartyBindMap::iterator it = party_bind_map_.find( party_id );
			if( it != party_bind_map_.end() )
				instance_party_bind = it->second;
		}

		if( instance_party_bind != NULL )
		{
			InstanceSave* instance_save = instance_party_bind->getBoundInstance( map_id, difficulty );
			if( instance_save != NULL )
			{
				if( party_bind != NULL )
					*party_bind = cTRUE;

				return instance_save;
			}
		}
	}

	return NULL;
}

InstanceSave* InstanceMapManager::findInstanceSave( tINT map_id, tINT channel_id, tINT instance_id )
{
	// 인던 객체를 얻는다
	std::pair<ContainerMapList::iterator, ContainerMapList::iterator> ret = map_list_.equal_range( map_id );
	for( ContainerMapList::iterator it = ret.first; it != ret.second; ++it )
	{
		if( it->second->getChannelId() == channel_id )
		{
			ContainerMap* container_map = it->second;

			return container_map->findInstnace( instance_id );
		}
	}

	return NULL;
}

void InstanceMapManager::removeMap( tINT map_id, tINT channel_id )
{
	// 컨테이너와 접속이 끊길 떄 호출됨
	std::pair<ContainerMapList::iterator, ContainerMapList::iterator> ret = map_list_.equal_range( map_id );
	for( ContainerMapList::iterator it = ret.first; it != ret.second; ++it )
	{
		if( it->second->getChannelId() == channel_id )
		{
			ContainerMap* container_map = it->second;
			container_map->removeMap( this );

			LOG_INFO( "InstanceMapManager. remove map. map: %d, channel: %d", map_id, channel_id );
			delete container_map;
			map_list_.erase( it );
			break;
		}
	}
}


void InstanceMapManager::clearInstanceSave( InstanceSave* instance_save )
{
	// 인던이 삭제되기 전에 인던에 귀속된 유저, 파티를 귀속 해제 시킨다

	{
		InstanceSave::UserList::iterator it = instance_save->user_list_.begin();
		InstanceSave::UserList::iterator end = instance_save->user_list_.end();
		while( it != end )
		{
			InstanceSave::UserList::iterator curr = it++;
			unbindInstanceUser( instance_save->map_id_, instance_save->channel_id_, instance_save->difficulty_, instance_save->instance_id_, (*curr) );
		}
	}

	{
		InstanceSave::PartyList::iterator it = instance_save->party_list_.begin();
		InstanceSave::PartyList::iterator end = instance_save->party_list_.end();
		while( it != end )
		{
			InstanceSave::PartyList::iterator curr = it++;
			unbindInstanceParty( instance_save->map_id_, instance_save->channel_id_, instance_save->difficulty_, instance_save->instance_id_, (*curr) );
		}
	}
}

void InstanceMapManager::ContainerMap::removeMap( InstanceMapManager* manager )
{
	// 채널안에 있는 인던을 전부 삭제 시킨다
	InstancesMap::iterator end = instances_map_.end();
	for( InstancesMap::iterator it = instances_map_.begin(); it != end; ++it )
	{
		InstanceSave* instance_save = it->second;

		manager->clearInstanceSave( instance_save );

		LOG_INFO( "InstanceMapManager. destroy. map: %d, channel: %d, instance: %d, count: %d",
			instance_save->map_id_, instance_save->channel_id_, instance_save->instance_id_, getInstanceCount() );
		delete instance_save;
	}

	instances_map_.clear();
}
