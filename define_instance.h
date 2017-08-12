#pragma once

struct TInstanceInfo
{
	tINT map_id_;
	tINT channel_id_;
	tINT difficulty_;
	tINT instance_id_;
	tINT cooltime_;
	tINT unload_time_;
};

enum InstanceError
{
	INSTANCE_NO_ERROR,						// 성공
	INSTANCE_ERROR_HAVE_COOLTIME,			// 쿨타밍 있음
	INSTANCE_ERROR_ALREADY_RESET,			// 이미 리셋되었음
	INSTANCE_ERROR_RESET_COUNT_LIMIT,		// 리셋 횟수 초과
	INSTANCE_ERROR_RESET_NOT_HAVE_ITEM,		// 리셋 아이템 없음
};
