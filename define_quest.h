#pragma once

#define MAX_QUEST					(24)
#define STATUS_QUEST_EMPTY			(0)

#define			QUEST_TITLE_MAX							(60 + 1)
#define			QUEST_RECOMPENSE_NORMAL_ITEM_MAX		(3)
#define			QUEST_RECOMPENSE_SELECT_ITEM_MAX		(12)
#define			QUEST_REGION_NAME_MAX					(16 + 1)
#define			QUEST_MISSION_CLEAR_TITLE_MAX			(60 + 1)
#define			QUEST_FLAG_MAX							(5)
#define			QUEST_MISSION_TITLE_MAX					(60 + 1)
#define			QUEST_GIVE_ITEM_MAX						(3)
#define			QUEST_GIVE_BUFF_MAX						(3)
#define			QUEST_NEED_QUEST_MAX					(5)
#define			QUEST_WEBPAGE_MAX						(256)
#define			QUEST_UNPAYBACK_MAX						(6)
#define			QUEST_DAILY_GROUP_MAX					(8)
#define			QUEST_BROKER_OWN_MAX					(1)
#define			QUEST_BROKER_FLAG_MAX					(3)

enum EQuestSort
{
	QUEST_SORT_KARMA = 0,					// 카르마 협행
	QUEST_SORT_NORMAL = 1,					// 일반 협행
	QUEST_SORT_SYSTEM = 2,					// 시스템 협행
	QUEST_SORT_BROKER = 3,					// 거간꾼 퀘스트
};

enum ENeedQuestType
{
	NEED_QUEST_TYPE_OR,						// 필요 퀘스트 조건 or
	NEED_QUEST_TYPE_AND,					// 필요 퀘스트 조건 and
	NEED_QUEST_TYPE_MAX
};


#define			KARMA_QUEST_COUNT						( 1 )
#define			NORAML_QUEST_COUNT						( 20 )
#define			SYSTEM_QUEST_COUNT						( 3 )

enum EQuestPacketState {

	BEGIN_KARMA_QUEST,
	BEGIN_NORMAL_QUEST,
	BEGIN_SYSTEM_QUEST,

	BEGIN_FORCED_KARMA_QUEST,
	BEGIN_FORCED_NORMAL_QUEST,
	BEGIN_FORCED_SYSTEM_QUEST,

	UPDATE_QUEST,
	UPDATE_COMPLETE_STEP,

	END_FAIL_QUEST,
	END_COMPLETE_ALL,
	END_TIMEOUT_QUEST,

	ERR_QUEST_BROKER_QUEST_ERR_NONE,			// 거간꾼 퀘스트 에러 없음
	END_QUEST_BROKER_QUEST_TIMEOUT,				// 제한 시간 종료로 거간꾼 퀘스트 종료

	ERR_QUEST_PACKET = 100,
	ERR_QUEST_NOT_FOUND_QUEST_ID,				// 퀘스트 없음
	ERR_QUEST_NOT_FOUND_NPC_ID,					// NPC 없음
	ERR_QUEST_MIN_LEVEL,						// 시작 조건이 안됨 - 최소 레벨
	ERR_QUEST_MAX_LEVEL,						// 시작 조건이 안됨 - 최대 레벨
	ERR_QUEST_JOB,								// 시작 조건이 안됨 - 직업
	ERR_QUEST_KIND,								// 시작 조건이 안됨 - 성향
	ERR_QUEST_MIN_GUILD_POSITION,				// 시작 조건이 안됨 - 최소 길드등급
	ERR_QUEST_MAX_GUILD_POSITION,				// 시작 조건이 안됨 - 최대 길드등급
	ERR_QUEST_PK_COUNT,							// 시작 조건이 안됨 - PK 수
	ERR_QUEST_KARMA,							// 시작 조건이 안됨 - 카르마 단계
	ERR_QUEST_NOT_HAVE_COMPELTE,				// 시작 조건이 안됨 - 완료 퀘스트가 없음
	ERR_QUEST_ALREADY_COMPLETED,				// 이미 완료한 퀘스트
	ERR_QUEST_ALREADY_HAVE,						// 이미 수행중인 퀘스트
	ERR_QUEST_FULL,								// 12개가 다 찼음
	ERR_QUEST_NOT_ENOUGH_COIN,					// 돈이 부족함
	ERR_QUEST_NOT_HAVE_QUEST,					// 퀘스트를 진행중이 아님
	ERR_QUEST_NOT_FOUND_RANDOM_QUEST_NPC_ID,	// RandomQuest Npcid 없음
	ERR_QUEST_INVALUD_RANDOM_QUEST_RANK,		// rank 가 틀렸음
	ERR_QUEST_INVALUD_RANDOM_QUEST_INDEX,		// index 가 틀렸음
	ERR_QUEST_IMPOSSIBLE_GIVEUP,				// 포기할 수 없는 퀘스트임
	ERR_QUEST_NOT_FOUND_SALE_QUEST_ID,			// 아이템 보상받는 퀘스트 아이디 없음 (npc script에 적힌 sale quest id가 없는 퀘스트임)
	ERR_QUEST_TALK_EVENT_WRONG_NPC_QUEST_ID,	// 아이템 보상받는 NPC와 내 퀘스트의 ID가 틀림
	ERR_QUEST_TALK_EVENT_WRONG_FALGS,			// 완료조건 Flags가 틀림
	ERR_QUEST_WRONG_STEP,						// 단계가 틀림
	ERR_QUEST_INVENTORY_FULL,					// 인벤토리 풀
	ERR_QUEST_ADD_ITEM_ERROR,					// 아이템 넣기 실패
	ERR_QUEST_KARMA_QUEST_FULL,					// 카르마 퀘스트 풀
	ERR_QUEST_NORMAL_QUEST_FULL,				// 일반 퀘스트 풀
	ERR_QUEST_SYSTEM_QUEST_FULL,				// 시스템 퀘스트 풀
	ERR_QUEST_NOT_SELECT_REWARD_ITEM,			// 보상 아이템 선택을 안했음
	ERR_QUEST_INVALID_SELECT_REWARD_ITEM,		// 보상 아이템 인덱스 잘못 입력
	ERR_QUEST_NOT_FOUND_SELECT_REWARD_ITEM,		// 보상 아이템 없음
	ERR_QUEST_FORCED_FULL,						// 강제 협행중 12개가 다 찼음
	ERR_QUEST_FORCED_KARMA_QUEST_FULL,			// 강제 협행중 카르마 퀘스트 풀
	ERR_QUEST_FORCED_NORMAL_QUEST_FULL,			// 강제 협행중 일반 퀘스트 풀
	ERR_QUEST_FORCED_SYSTEM_QUEST_FULL,			// 강제 협행중 시스템 퀘스트 풀
	ERR_QUEST_NOT_HAVE_QUEST_ITEM,				// 협행에 필요한 아이템이 없음
	ERR_QUEST_BUFF_TYPE,						// 버프 없음
	ERR_QUEST_NEED_SPECIAL_ABILITY_GREADE,		// 전문 능력치 부족
	ERR_QUEST_INVENTORY_FULL_NEXT_STEP,			// 인벤토리 풀로 다음 중임으로 못 넘어감
	ERR_QUEST_PENDING_ITEM_ACTION,				// 아이템 관련 행동 불가
	ERR_QUEST_REPRESS_QUEST_ACCEPT_TIME_END,	// 진압퀘스트 응답시간 초과
	ERR_QUEST_NOT_FOUND_LOOP_GROUP_LIMIT_SCRIPT,// 일일 반복 퀘스트 스크립트 없음
	ERR_QUEST_DAILY_QUEST_GROUP_RECEVIE_MAX,	// 더이상 해당 그룹의 일일 퀘스트를 받을 수 없음
	ERR_QUEST_WORLD_EVENT,
	ERR_QUEST_COMPLETED_WORLD_EVENT_QUEST,
	ERR_QUEST_UNABLE_PORTAL,					// 비활성 포탈(현재는 작동하지 않습니다.);
	ERR_QUEST_MAP_ID,							// 시작 조건이 안됨 - 맵 아이디

	ERR_QUEST_BROKER_NOT_USER_LEVEL,			// 유저 레벨 부족
	ERR_QUEST_BROKER_FAIL_GET_NEW_QUEST,		// 새로운 거간꾼 퀘스트 받기 실패
	ERR_QUEST_BROKER_NPC_NOT_FOUND,				// NPC가 없음
	ERR_QUEST_BROKER_ALREADY_ACCEPT_QUEST,		// 이미 받은 거간꾼 퀘스트가 있음
	ERR_QUEST_BROKER_TOO_LONG_DISTANCE,			// NPC와의 거리가 너무 멈
	ERR_QUEST_BROKER_NPC_KIND_WRONG,			// NPC 종류가 거간꾼이 아님
	ERR_QUEST_BROKER_REQUEST_NOT_YET,			// 거간꾼 퀘스트 Request가 아직 되지 않았음.
	ERR_QUEST_BROKER_NOT_FOUND_QUEST_SCRIPT,	// 퀘스트 스크립트가 없음
	ERR_QUEST_BROKER_QUEST_FULL,				// 거간꾼 퀘스트 풀
	ERR_QUEST_FORCED_BROKER_QUEST_FULL,			// 강제 협행중 거간꾼 퀘스트 풀
	ERR_QUEST_BROKER_NOT_FOUND,					// 등록된 거간꾼 퀘스트가 없음
	ERR_QUEST_BROKER_NOT_ACCEPT_YET,			// 아직 수락된 퀘스트가 아님
	ERR_QUEST_BROKER_SELECT_OVER_6,				// 선택된 퀘스트의 수가 6개를 초과함
};
