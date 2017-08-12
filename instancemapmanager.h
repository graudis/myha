#pragma once

// 인던 하나를 나타냄
class InstanceSave
{
public:
	typedef UNORDERED_SET<tINT> UserList;
	typedef UNORDERED_SET<tINT> PartyList;

	InstanceSave(tINT map_id, tINT channel_id, tINT instance_id, tINT difficulty);
	~InstanceSave();

	void addUser(tINT user_id) { user_list_.insert(user_id); }
	void addParty(tINT party_id) { party_list_.insert(party_id); }

	void removeUser(tINT user_id) { user_list_.erase(user_id); }
	void removeParty(tINT party_id) { party_list_.erase(party_id); }

	tBOOL hasUser(tINT user_id) { return user_list_.find(user_id) != user_list_.end(); }
	tBOOL hasParty(tINT party_id) { return party_list_.find(party_id) != party_list_.end(); }

	void setDestroyEndTime(tUINT destroy_end_time) { destroy_end_time_ = destroy_end_time; }

	const UserList& getUserList() { return user_list_; }

	tINT map_id_;
	tINT channel_id_;
	tINT instance_id_;
	tINT difficulty_;
	tINT destroy_end_time_;

	UserList user_list_;			// 인던에 귀속되어 있는 유저 리스트
	PartyList party_list_;			// 인던에 귀속되어 있는 파티 리스트
};

// 유저가 귀속된 인던 리스트
class InstanceUserBind
{
public:
	InstanceUserBind(tINT user_id) : user_id_(user_id) {}
	~InstanceUserBind() {}

	void bindInstance(tINT map_id, InstanceSave* instance_save);
	void unbindInstance(tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id);
	void unbindInstanceAll();

	InstanceSave* getBoundInstance(tINT map_id, tINT difficulty);

	tINT user_id_;

	typedef std::multimap<tINT, InstanceSave*> BoundInstancesMap;	// map_id, InstanceSave*
	BoundInstancesMap boundInstances_;			// 유저가 귀속되어 있는 인던맵 리스트
};

// 파티가 귀속된 인던 리스트
class InstancePartyBind
{
public:
	InstancePartyBind(tINT party_id) : party_id_(party_id) {}
	~InstancePartyBind() {}

	void bindInstance(tINT map_id, InstanceSave* instance_save);
	void unbindInstance(tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id);
	InstanceSave* getBoundInstance(tINT map_id, tINT difficulty);

	tINT party_id_;

	typedef std::multimap<tINT, InstanceSave*> BoundInstancesMap;	// map_id, InstanceSave*
	BoundInstancesMap boundInstances_;			// 파티가 귀속되어 있는 인던맵 리스트
};

class InstanceMapManager
{
public:
	InstanceMapManager();
	~InstanceMapManager();

	tBOOL createInstance(tINT map_id, tINT channel_id, tINT instance_id, tINT difficulty);
	void destroyInstance(tINT map_id, tINT channel_id, tINT instance_id);

	InstanceSave* bindInstance(tINT map_id, tINT channel_id, tINT instance_id, tINT user_id, tINT party_id, tBOOL* user_bind);

	void unbindInstanceUser(tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id, tINT user_id);
	void unbindInstanceParty(tINT map_id, tINT channel_id, tINT difficulty, tINT instance_id, tINT party_id);

	void unbindInstanceAll(tINT user_id);

	InstanceSave* findBoundInstanceSave(tINT map_id, tINT difficulty, tINT user_id, tINT party_id, tBOOL* party_bind = NULL);
	InstanceSave* findInstanceSave(tINT map_id, tINT channel_id, tINT instance_id);

	void removeMap(tINT map_id, tINT channel_id);

private:
	void clearInstanceSave(InstanceSave* instance_save);

private:
	class ContainerMap
	{
	public:
		ContainerMap(tINT map_id, tINT channel_id) : map_id_(map_id), channel_id_(channel_id) {}
		~ContainerMap() {}

		tINT getMapId() { return map_id_; }
		tINT getChannelId() { return channel_id_; }

		void addInstance(InstanceSave* instance_save)
		{
			instances_map_.insert(InstancesMap::value_type(instance_save->instance_id_, instance_save));
		}

		void removeInstance(tINT instance_id)
		{
			instances_map_.erase(instance_id);
		}

		InstanceSave* findInstnace(tINT instance_id)
		{
			InstancesMap::iterator it = instances_map_.find(instance_id);
			if (it == instances_map_.end())
				return NULL;

			return it->second;
		}

		void removeMap(InstanceMapManager* manager);

		tINT getInstanceCount() { return instances_map_.size(); }

	private:
		tINT map_id_;
		tINT channel_id_;

		typedef UNORDERED_MAP<tINT, InstanceSave*> InstancesMap;
		InstancesMap instances_map_;		// 해당 채널에 생성된 인던 리스트
	};

	typedef std::multimap<tINT, ContainerMap*> ContainerMapList;		// map_id, ContainerMap*
	typedef UNORDERED_MAP<tINT, InstanceUserBind*> InstanceUserBindMap;		// user_id, InstanceUserBind*
	typedef UNORDERED_MAP<tINT, InstancePartyBind*> InstancePartyBindMap;	// party_id, InstancePartyBind*

	ContainerMapList map_list_; // 전체맵 인던 리스트
	InstanceUserBindMap user_bind_map_; // 유저별 귀속 리스트
	InstancePartyBindMap party_bind_map_; // 파티별 귀속 리스트
};
