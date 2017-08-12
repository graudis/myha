#include "stdafx.h"

#include "mservercmd.h"

#include "myhaMaster.h"
#include "service.h"

MServerCmd::MServerCmd()
{
}

MServerCmd::~MServerCmd()
{
    clear();
}

MServerCmd* MServerCmd::instance()
{
	static MServerCmd cmd;
	return &cmd;
}

MServerCmdService* MServerCmd::lookup(const tCHAR* ip)
{
	std::string daemonIp = ip;
	map_t::iterator it = connect_list_.find(daemonIp);
	if (it == connect_list_.end())
		return NULL;
	else
		return it->second;
}

void MServerCmd::clear()
{
	connect_list_.clear();
}

tBOOL MServerCmd::createService(const tCHAR* ip)
{
	if ( lookup( ip ) != NULL )
		return cTRUE;

	MServerCmdService* cmd = new MServerCmdService;

	session_handle chanlde = bnf::instance()->CreateConnect(ip, SERVER_CMD_PORT, cmd);
	if (chanlde == SessionBase::INVALID_SESSION_HANDLE)
	{
		LOG_ERROR( "CreateConnect(%s,%d) error", ip, SERVER_CMD_PORT);
		return cFALSE;
	}

	rnSocketIOService* service = (rnSocketIOService*)bnf::instance()->GetSessionPointer(chanlde);
	cmd->init(service, ip);

	connect_list_.insert(map_t::value_type(ip, cmd));
	return cTRUE;
}

void MServerCmd::deleteService(const tCHAR* ip)
{
	MServerCmdService* cmd = lookup(ip);
	if (cmd == NULL)
	{
		LOG_ERROR("delete error(%s)", ip);
		return;
	}

	connect_list_.erase(ip);
	delete cmd;
}
