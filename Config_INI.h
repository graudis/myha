#pragma once

namespace Config
{
	class INI
	{
	public:
		INI();
		~INI();

		static INI* Instance()
		{
			static INI inst;
			return &inst;
		}

		bool init(std::string file_name);

		std::string getVirtualIP()
		{
			return __dbvip;
		}

		std::string getVirtualIPNIC()
		{
			return __dbvip_nic;
		}

		std::string getMasterIP()
		{
			return __master_ip;
		}

		std::string getSlaveIP()
		{
			return __slave_ip;
		}

	private:
		std::string __dbvip;
		std::string __dbvip_nic;

		std::string __master_ip;
		std::string __master_id;
		std::string __master_pw;

		std::string __slave_ip;
		std::string __slave_id;
		std::string __slave_pw;
	};
}
