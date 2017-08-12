#pragma once

#include <sys/time.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

enum TimeConstants
{
	TIME_SECOND = 1,
	TIME_MINUTE = TIME_SECOND * 60,
	TIME_HOUR = TIME_MINUTE * 60,
	TIME_DAY = TIME_HOUR * 24,
	TIME_MONTH = TIME_DAY * 30,
	TIME_YEAR = TIME_MONTH * 12,
};

enum MsTimeConstants
{
	MSTIME_SECOND = 1000,
	MSTIME_MINUTE = MSTIME_SECOND * 60,
	MSTIME_HOUR = MSTIME_MINUTE * 60,
	MSTIME_DAY = MSTIME_HOUR * 24,
};


////////////////////////////////////////////////////////////////////////////////////////////////////
// time function
inline tUINT getMSTime()
{
	struct timeval tv;
	struct timezone tz;
	gettimeofday( &tv, &tz );
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}


inline tINT getMSTimeDiff( tUINT oldMSTime, tUINT newMSTime )
{
	if( newMSTime == 0 )
		return -1;

	return newMSTime - oldMSTime;
}


#define Sleep( ms_time )			usleep( ms_time * 1000 )
