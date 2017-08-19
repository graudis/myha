#pragma once

#define IP_STRING_MAX (15 + 1)

#define STRNCPY(x,y,z)\
do\
{\
	strncpy(x, y, z);\
	x[z - 1] = '\0';\
} while (0)
