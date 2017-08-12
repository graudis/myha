#pragma once

#include <map>

#include "mservercmd_service.h"
#include "myhaMaster.h"

class MServerCmd
{
public:
	typedef UNORDERED_MAP<std::string, MServerCmdService*> map_t;

	MServerCmd();
	~MServerCmd();

public:
	static MServerCmd*				instance();

	tBOOL			          		createService(const tCHAR* ip);
	void							deleteService(const tCHAR* ip);

	tUINT							size() { return connect_list_.size(); }
	MServerCmdService*				lookup(const tCHAR* ip);

	void							clear();

public:
	map_t							connect_list_;
};
