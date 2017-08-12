#include "stdafx.h"

#include "Config_INI.h"

Config::INI::INI()
{
}

Config::INI::~INI()
{
}

bool Config::INI::init(std::string file_name)
{
	boost::property_tree::ptree pt;

	try
	{
		boost::property_tree::ini_parser::read_ini(file_name, pt);
	}
	catch (boost::property_tree::ini_parser_error& err)
	{
		std::cout << err.message() << "'" << err.filename() << "'." << std::endl;
		std::cout << "line : " << err.line() << std::endl;
		return false;
	}

	__dbvip = pt.get<std::string>("dbvip");
	__dbvip_nic = pt.get<std::string>("dbvip_nic");
	__master_ip = pt.get<std::string>("master_ip");
	__slave_ip = pt.get<std::string>("slave_ip");

//	__master_id = pt.get<std::string>("master_id");
//	__master_pw = pt.get<std::string>("master_pw");
//	__slave_id = pt.get<std::string>("slave_id");
//	__slave_pw = pt.get<std::string>("slave_pw");

	// std::cout << __dbvip << std::endl;
	// std::cout << __dbvip_nic << std::endl;
	// std::cout << __ha_node1 << std::endl;
	// std::cout << __ha_node2 << std::endl;
	// std::cout << __master_id << std::endl;
	// std::cout << __master_pw << std::endl;
	// std::cout << __slave_id << std::endl;
	// std::cout << __slave_pw << std::endl;

	return true;
}
