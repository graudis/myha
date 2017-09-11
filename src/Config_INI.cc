#include "stdafx.h"

#include "Config_INI.h"

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

	return true;
}
