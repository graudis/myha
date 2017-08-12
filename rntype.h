#pragma once

#include <stdio.h>
#include <limits.h>
#include <time.h>

typedef char					tCHAR;
typedef unsigned char		tUCHAR;
typedef unsigned char		tBYTE;
typedef short int				tSINT;
typedef unsigned short int	tUSINT;
typedef int					tINT;
typedef unsigned int			tUINT;
typedef long					tLONG;
typedef unsigned long		tULONG;
typedef float					tFLOAT;
typedef double				tDOUBLE;
typedef bool					tBOOL;
typedef long long				tINT64;
typedef unsigned long long	tUINT64;
typedef time_t				tTIME;
typedef size_t				tSIZE;

const tBOOL				cFALSE		= false;
const tBOOL				cTRUE		= true;
const tCHAR				cMIN_CHAR	= SCHAR_MIN;
const tCHAR				cMAX_CHAR	= SCHAR_MAX;
const tBYTE				cMIN_BYTE	= 0;
const tBYTE				cMAX_BYTE	= UCHAR_MAX;
const tFLOAT			cMIN_FLOAT	= 1.175494351e-38F;
const tFLOAT			cMAX_FLOAT	= 3.402823466e+38F;
const tDOUBLE			cMIN_DOUBLE	= 2.2250738585072014e-308;
const tDOUBLE			cMAX_DOUBLE	= 1.7976931348623158e+308;
const tSINT				cMIN_SINT	= SHRT_MIN;
const tSINT				cMAX_SINT	= SHRT_MAX;
const tUSINT			cMIN_USINT	= 0;
const tUSINT			cMAX_USINT	= USHRT_MAX;
const tINT				cMIN_INT	= INT_MIN;
const tINT				cMAX_INT	= INT_MAX;
const tUINT				cMIN_UINT	= 0;
const tUINT				cMAX_UINT	= UINT_MAX;
const tLONG				cMIN_LONG	= LONG_MIN;
const tLONG				cMAX_LONG	= LONG_MAX;
const tULONG			cMIN_ULONG	= 0;
const tULONG			cMAX_ULONG	= ULONG_MAX;
const tINT				cEOF		= -1;
const tCHAR				cEOL		= '\n';
const tCHAR				cEOS		= '\0';


typedef bool					BOOL;
typedef unsigned char			BYTE;
typedef unsigned short int		WORD;
typedef unsigned int			DWORD;
typedef DWORD					RET_VAL;
typedef void*					LPVOID;


#define IP_STRING_MAX			  (15 + 1)

#define STRNCPY(x,y,z)			do \
{ \
	strncpy(x, y, z); \
	x[z - 1] = '\0'; \
} while (0)
