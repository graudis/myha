#include "stdafx.h"

#include <pwd.h>
#include <sys/types.h>

// #include "castislogger.h"

#include "network_util.h"

#include "ProcessCheck.h"

#include "myhaMaster.h"
#include "myhaSlave.h"

int main(int argc, char *argv[])
{
// 	castis::logger::init_logger("example", "1.0.0");
// 	CILOG(foo) << "Just a foo";
// 	CILOG(debug) << "A normal severity message";
// 	CILOG(report) << "A notification severity message";
// 	CILOG(warning) << "A warning severity message";
// 	CILOG(info) << "A information severity message";
// 	CILOG(error) << "A error severity message";
// 	CILOG(fail) << "A fail severity message";
// 	CILOG(success) << "A success severity message";
// 	CILOG(exception) << "A exception severity message";
// 	CILOG(critical) << "A critical severity message";
// 
// 	return 0;

	LogSystem::setSubstitutedValuef( "logfile", "myha" );
	LogSystem::configureXml( "../cfg/log.xml" );

	struct passwd* pw = getpwuid(getuid());
	if (pw->pw_uid != 0 || pw->pw_gid != 0)
	{
		printf("관리자 권한이 아닙니다.\n");
		printf("관리자 권한으로 실행시키십시오.\n");
//		return 1;
	}

	if (Config::INI::Instance()->init("config.ini") == false)
	{
		printf("Can not load 'config.ini' file.\n");
		return 2;
	}

	std::string host_ip = getLocalIP(Config::INI::Instance()->getVirtualIPNIC());

	LOG_TRACE("host_ip=[ %s ] MasterIP=[ %s ]", host_ip.c_str(), Config::INI::Instance()->getMasterIP().c_str());
	LOG_TRACE("host_ip=[ %s ] Slave IP=[ %s ]", host_ip.c_str(), Config::INI::Instance()->getSlaveIP().c_str());

	if (host_ip == Config::INI::Instance()->getMasterIP())
	{
		LOG_TRACE("I am Master.");

		if (myhaMaster::config() == false)
		{
			printf( "[main]\t configuration error \n" );
			return -1;
		}

//		system("ip addr add 192.168.122.200/32 dev eth0");

		if (myhaMaster::run() == false)
			return -1;
	}
	else if (host_ip == Config::INI::Instance()->getSlaveIP())
	{
		LOG_TRACE("I am Slave.");
		if (myhaSlave::init(1) == false)
		{
			printf( "[main]\t configuration error \n" );
			return -1;
		}

		if (myhaSlave::run() == false)
			return -1;
	}

	return 0;
}
