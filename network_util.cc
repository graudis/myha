#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ifaddrs.h>

#include <iostream>

#include "network_util.h"

tBOOL getLocalIp(tCHAR* buff, tINT buff_len)
{
	tCHAR host_name[256];

	gethostname(host_name, 256);

	struct hostent* hostinfo = gethostbyname(host_name);
	if (!hostinfo)
		return cFALSE;

	if (!hostinfo->h_addr_list)
		return cFALSE;

	tCHAR** addrs = hostinfo->h_addr_list;

	STRNCPY(buff, inet_ntoa(*(struct in_addr*)*addrs), buff_len);
	return cTRUE;
}

std::string getLocalIP(std::string if_name)
{
	struct ifaddrs * ifAddrStruct = NULL;
	struct ifaddrs * ifa = NULL;
	void * tmpAddrPtr = NULL;

	getifaddrs(&ifAddrStruct);

	for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (!ifa->ifa_addr)
		{
			continue;
		}
		if (ifa->ifa_addr->sa_family == AF_INET)	// check it is IP4
		{
			// is a valid IP4 Address
			tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

			std::string ifa_name = ifa->ifa_name;

			if (if_name == ifa->ifa_name)
				return addressBuffer;
		}
	}

	return "";
}
