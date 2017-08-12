#pragma once

#include "rntype.h"

#include "unordered_map_ts.h"
#include <string>
#include <vector>

class rnIpData
{
public:
	struct TIpData
	{
		std::string		private_;
		std::string		public_;
	};

	rnIpData();
	~rnIpData();

	tBOOL		open( const tCHAR* filename );
	tBOOL		clear();

	const tCHAR*	getPrivateAddress( const tCHAR* public_ip );
	const tCHAR*	getPublicAddress( const tCHAR* private_ip );

	const tCHAR*	getPrivateAddress()						{ return private_ip_.c_str();	}
	const tCHAR*	getPublicAddress()						{ return public_ip_.c_str();	}

	const tCHAR*		getListenIPAddress( tBOOL use_private_network );

private:
	tBOOL		isPrivateAddress( const tCHAR* ip );
	tBOOL		isPublicAddress( const tCHAR* ip );


private:
	std::vector<TIpData* >						ip_data_;

	unordered_map_ts<std::string, TIpData* >	private_ip_map_;
	unordered_map_ts<std::string, TIpData* >	public_ip_map_;

	std::string					private_ip_;
	std::string					public_ip_;


};
