#pragma once

#define MAX_MISSIONMAP_NAME_LENGTH			128
#define MAX_REWARD_BUFF						3
#define MAX_REWARD_ITEM						3
#define MAX_MISSIONMAP_ZONE					12
#define MAX_RANK_REWARD_COUNT				30
#define MAX_MISSION_STEP					30
#define MAX_START_LOCATION					20
#define MAX_REVIVE_BUFF						3
#define INVALID_ZONE_INDEX					255

#define MAX_MISSION_RANK_REWARD				7
#define FAIL_MISSION_RANK					6

enum MissionMapStatus
{
	MISSIONMAP_STATUS_NONE = 0,
	MISSIONMAP_STATUS_OPEN = 1,		// 맵 열림 - 유저 들어오고 있는 상태
	MISSIONMAP_STATUS_READY = 2,		// 준비 - 아이템 구입등 준비
	MISSIONMAP_STATUS_SETTING = 3,		// 게임 시작 전 연출 시간
	MISSIONMAP_STATUS_PLAY = 4,		// 게임중
	MISSIONMAP_STATUS_END_PLAY = 5,		// 게임 종료 - 보상지급
	MISSIONMAP_STATUS_CLOSE = 6,		// 맵 종료 - 참가자 원래자리로
};

// 설정파일에서 설정되지만, 디폴트 값 정의
#define MISSIONMAP_DEFAULT_MAX_PLAYER			360
#define MISSIONMAP_DEFAULT_ZONE_MAX_PLAYER		30
#define MISSIONMAP_DEFAULT_MIN_PLAYER			8

enum MissionMapDefaultTimeInfo
{
	OPEN_TIME = 5,			// seconds
	READY_TIME = 120,			// seconds
	SETTING_TIME = 10,			// seconds
	PLAY_TIME = 1200,			// seconds
	REARD_TIME = 30,			// seconds
	CLOSE_TIME = 10,			// seconds
};


enum MissionMapPacketState
{
	MISSIONMAP_STATE_SUCCESS = 0,

	MISSIONMAP_STATE_ALREADY_IN_QUEUE,			// 큐에 이미 들어가 있음
	MISSIONMAP_STATE_NOT_OPEN_STATUS,			// 입장 중 상태가 아님
	MISSIONMAP_STATE_ALREADY_IN_MAP,			// 이미 맵 들어가 있음
	MISSIONMAP_STATE_FALIED_MIN_LEVEL,			// 최소 레벨 조건이 안됨
	MISSIONMAP_STATE_FALIED_MAX_LEVEL,			// 최대 레벨 조건이 안됨
	MISSIONMAP_STATE_MAP_IS_FULL,				// 미션 맵이 가득 차 있음

	MISSIONMAP_STATE_NOT_IN_QUEUE,				// 큐에 들어가 있지 않음
	MISSIONMAP_STATE_NOT_IN_MAP,				// 맵에 들어가 있지 않음

	MISSIONMAP_STATE_NOT_EXIST_ZONE,			// 입장 가능한 존이 없음
	MISSIONMAP_STATE_WANTED_PLAYER,				// 현상범은 입장 불가

	MISSIONMAP_STATE_FALIED_COIN,				// 돈 부족
	MISSIONMAP_STATE_FALIED_FAME,				// 명성 부족
	MISSIONMAP_STATE_FALIED_OBSERVER,			// 관람 입장 실패
	MISSIONMAP_STATE_FALIED_FREE_MATCH,			// 이미 시드 있음. 프리매치 입장 불가
	MISSIONMAP_STATE_FALIED_RANKING_MATCH,		// 시드 없음. 랭킹매치 입장 불가
};

enum MissionEndState
{
	MISSION_END_COMPLETED = 1,	// 미션 완료
	MISSION_END_FAILED = 2,	// 미션 실패
	MISSION_END_NOT_ENOUGH_PLAYERS = 3,	// 플레이어 부족

	MISSION_END_CLOSED = 4,	// 미션 끝

	MISSION_END_BONUS = 5,	// 미션 보너스
};


enum MissionZoneStatus
{
	MISSION_ZONE_STATUS_NONE = 1,
	MISSION_ZONE_STATUS_READY = 2,
	MISSION_ZONE_STATUS_PLAY = 3,
	MISSION_ZONE_STATUS_WAIT = 4,
	MISSION_ZONE_STATUS_BONUS = 5,
	MISSION_ZONE_STATUS_END_PLAY = 6,
	MISSION_ZONE_STATUS_CLOSE = 7,
};

#define MISSION_WINNER_MAIL_SUBJECT		"투신제전 승리 보상"
#define MISSION_LOSER_MAIL_SUBJECT		"투신제전 참가 보상"
#define MISSION_MIN_PLAYERS_MAIL_SUBJECT	"투신제전 매칭 실패 보상"
#define MISSION_MIN_PLAYERS_MAIL_CONTENT	"매칭 최소 인원이 되지 않아 투신제전이 취소됩니다. 보상 아이템이 지급됩니다"
