#pragma once

#include "type.h"

bool getLocalIp(char* buff, int32_t buff_len);

std::string getLocalIP(std::string if_name);

bool checkVIP(std::string if_name, std::string ip_addr);
