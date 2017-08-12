#include <string.h>

#include "bnf.h"

#include "network_util.h"

#include "rnipdata.h"

#define MAX_LINE_BUFFER				8192


rnIpData::rnIpData()
{
}

rnIpData::~rnIpData()
{
	clear();
}

tBOOL rnIpData::open( const tCHAR* filename )
{
	FILE* fp = fopen( filename, "rt" );
	if( NULL == fp )
		return cFALSE;

	boost::shared_ptr<FILE> _auto_close(fp, fclose);

	tCHAR line_buffer[MAX_LINE_BUFFER];

	while( NULL != fgets( line_buffer, MAX_LINE_BUFFER, fp ) ) {

		// 주석
		tCHAR* comment = strchr( line_buffer, '#' );
		if( NULL != comment )
			line_buffer[comment - line_buffer] = '\0';

		// 구분자
		tCHAR* dilimeter = strchr( line_buffer, ':' );
		if( NULL == dilimeter )
			continue;

		// 라인 끝 문자
		tCHAR* line_end = strchr( dilimeter + 1, ';' );
		if( NULL != line_end )
			line_buffer[ line_end - line_buffer] = '\0';

		TIpData* ip_data = new TIpData;
		ip_data->private_ = std::string( line_buffer, (dilimeter - line_buffer) );
		ip_data->public_ = std::string( dilimeter + 1, (line_end - (dilimeter + 1)) );

		// 양 끝에 공백 제거
		tCHAR	space[] = " \t\r\n";

		ip_data->private_.erase( 0, ip_data->private_.find_first_not_of(space) );
		ip_data->private_.erase( ip_data->private_.find_last_not_of(space) + 1 );

		ip_data->public_.erase( 0, ip_data->public_.find_first_not_of(space) );
		ip_data->public_.erase( ip_data->public_.find_last_not_of(space) + 1 );

		ip_data_.push_back( ip_data );

		// map에 저장
		private_ip_map_.insert( ip_data->private_, ip_data );
		public_ip_map_.insert( ip_data->public_, ip_data );
	}

	tCHAR host_ip[IP_STRING_MAX];
	getLocalIp( host_ip, IP_STRING_MAX );

	if( cTRUE == isPrivateAddress( host_ip ) )
	{
		const tCHAR* public_ip = getPublicAddress( host_ip );
		if( NULL == public_ip )
			return cFALSE;

		const tCHAR* private_ip = getPrivateAddress( public_ip );
		if( NULL == private_ip )
			return cFALSE;

		public_ip_ = std::string( public_ip );
		private_ip_ = std::string( private_ip );
	}
	else if( cTRUE == isPublicAddress( host_ip ) )
	{
		const tCHAR* private_ip = getPrivateAddress( host_ip );
		if( NULL == private_ip )
			return cFALSE;

		const tCHAR* public_ip = getPublicAddress( private_ip );
		if( NULL == public_ip )
			return cFALSE;

		public_ip_ = std::string( public_ip );
		private_ip_ = std::string( private_ip );
	}
	else
	{
		return cFALSE;
	}
	return cTRUE;
}

tBOOL rnIpData::clear()
{
	std::vector<TIpData* >::iterator low = ip_data_.begin();
	std::vector<TIpData* >::iterator high = ip_data_.end();
	for( ; low != high; ++low)
		delete (*low);

	ip_data_.clear();
	private_ip_map_.clear();
	public_ip_map_.clear();
	return cTRUE;
}

const tCHAR* rnIpData::getPrivateAddress( const tCHAR* public_ip )
{
	unordered_map_ts<std::string, TIpData* >::iterator it = public_ip_map_.find( public_ip );

	if( it == public_ip_map_.end() )
		return NULL;

	return (*it).second->private_.c_str();
}

const tCHAR* rnIpData::getPublicAddress( const tCHAR* private_ip )
{
	unordered_map_ts<std::string, TIpData* >::iterator it = private_ip_map_.find( private_ip );

	if( it == private_ip_map_.end() )
		return NULL;

	return (*it).second->public_.c_str();
}


tBOOL rnIpData::isPrivateAddress( const tCHAR* ip )
{
	unordered_map_ts<std::string, TIpData* >::iterator it = private_ip_map_.find( ip );

	return (it != private_ip_map_.end());
}

tBOOL rnIpData::isPublicAddress( const tCHAR* ip )
{
	unordered_map_ts<std::string, TIpData* >::iterator it = public_ip_map_.find( ip );

	return (it != public_ip_map_.end());
}

const tCHAR* rnIpData::getListenIPAddress( tBOOL use_private_network )
{
	if (use_private_network == cTRUE)	return private_ip_.c_str();
	else								return public_ip_.c_str();
}
