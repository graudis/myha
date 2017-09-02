#include "stdafx.h"

// c library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>

#include <dirent.h>
#include <pwd.h>

// c++ library
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <set>

// boost library
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/tokenizer.hpp>
#include <boost/thread.hpp>

#include "ProcessCheck.h"

bool TCPPortCheck(std::string check_ip, uint16_t check_port)
{
	std::ifstream in_stream;
	std::string read_line;

	in_stream.open("/proc/net/tcp");

	for (int32_t line = 0; in_stream.eof() == false; ++line)
	{
		std::getline(in_stream, read_line);

		if (line < 1)
			continue;

		typedef boost::tokenizer< boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep(" ");
		tokenizer tokens(read_line, sep);

		std::vector<std::string> xxx;
		for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
			xxx.push_back(*tok_iter);

		if (xxx.size() <= 1)
			continue;

		if (xxx.size() > 1)
		{
			boost::char_separator<char> sep(":");
			tokenizer tokens2(xxx[1], sep);

			std::vector<std::string> xx2;
			for (tokenizer::iterator tok_iter2 = tokens2.begin(); tok_iter2 != tokens2.end(); ++tok_iter2)
				xx2.push_back(*tok_iter2);

			std::stringstream ss1;
			std::stringstream ss2;
			uint32_t ip;
			uint16_t port;

			ss1 << std::hex << xx2[0];
			ss1 >> ip;

			ip = ntohl(ip);

			ss2 << std::hex << xx2[1];
			ss2 >> port;

			struct sockaddr_in addr1;
			addr1.sin_addr.s_addr = htonl(ip);

			std::string str = inet_ntoa(addr1.sin_addr);

			if (check_ip != str)
				continue;

			if (check_port != port)
				continue;

			if (xxx[3] != "0A")
				continue;

			in_stream.close();
			return true;
		}
	}

	in_stream.close();
	return false;
}

bool ProcessCheck()
{
	std::string proc_name = "mysqld";

	DIR *dir;				//  /proc/pid/ 를 가리킬 DIR* 변수
	struct dirent *entry;	// 각 파일의 inode를 통해 파일을 선택할 dirent 구조체
	struct stat fileStat;	// 파일의 정보를 담는 구조체

	int pid;				// 프로세스는 /proc 디렉토리에 자신의 pid로 파일을 담아 둡니다.
	char cmdLine[256];
	char tempPath[256];
//	printf("Processes Info\n");

	dir = opendir("/proc");	//  /proc이란 디렉토리 스트름이 대한 포인터가 반환되었습니다.

	while ((entry = readdir(dir)) != NULL)
	{
		// /proc에 존재하는 파일들을 차례대로 읽습니다.
		lstat(entry->d_name, &fileStat);	// DIR*가 가리키는 파일의 state 정보를 가져온다.

		if (!S_ISDIR(fileStat.st_mode))		// is dir? 디렉토리인지 확인한다.
			continue;						// 프로세스는 /proc에 자신의 pid로 디렉토리를
											// 만드는 점을 안다면 이해하실거라 생각합니다.

		pid = atoi(entry->d_name);			// 프로세스(디렉토리)인것을 확인하면, 숫자로 반환한다.
		if (pid <= 0)
			continue;						// 숫자가 아니라면 다시 continue;

		sprintf(tempPath, "/proc/%d/cmdline", pid);		// cmdline :: 프로세스 이름이 적힌파일
		getCmdLine(tempPath, cmdLine);					// /proc/pid/cmdline에서 프로세스의 이름을
														// 가져오는 함수로 보냅니다. 아래에 정의되어있습니다.
		std::string str = cmdLine;
		if (str.length() == 0)
			continue;

		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep(" ");
		tokenizer tokens(str, sep);

		for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
		{
			std::string str2 = *tok_iter;
			boost::char_separator<char> sep2("//");
			tokenizer tokens2(str2, sep2);
			for (tokenizer::iterator tok_iter2 = tokens2.begin(); tok_iter2 != tokens2.end(); ++tok_iter2)
			{
				if (*tok_iter2 == proc_name)
				{
					closedir(dir);
					return true;
				}
			}
		}
	}

	closedir(dir);

	return false;
}

int getCmdLine(char *file, char buf[256])
{
	FILE *srcFp;
	srcFp = fopen(file, "r");	//   /proc/pid/cmdline에 이름이 있습니다.

	memset(buf, 0, 256);
	fgets(buf, 256, srcFp);
	fclose(srcFp);

	return 0;
}
