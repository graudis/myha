#pragma once

#include <string>
#include <vector>

int getCmdLine(char *file, char buf[256]);

bool TCPPortCheck(std::string check_ip, uint16_t check_port);
bool ProcessCheck();
