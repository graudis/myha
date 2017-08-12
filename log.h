#pragma once

#include "logger.h"
#include "log_reason.h"

enum GLOG_KEY
{
	GLOG_LOGIN,
	GLOG_LOGOUT,
	GLOG_MAPIN,
	GLOG_MAPOUT,
	GLOG_EXP,
	GLOG_LEVELUP,
	GLOG_REVIVEDEAD,
	GLOG_CREATEDELETE,
	GLOG_JACKPOT,
	GLOG_JACKPOT_POINT,
	GLOG_COIN,
	GLOG_FAME,
	GLOG_SKILL,
	GLOG_BODYPOINT,
	GLOG_QUEST,
	GLOG_GMCOMMAND,
	GLOG_GUILD_CREATEDELETE,
	GLOG_GUILD_MEMBER_ADDDEL,
	GLOG_GUILD_DEPOSIT,
	GLOG_EXCHANGE_ITEM,
	GLOG_MAKE_ITEM,
	GLOG_CURRENT_CONNECT_USERS,
	GLOG_CONNECT_USERS_COUNT_MAX,
	GLOG_ITEM_CREATE,
	GLOG_ITEM_ACTION,
	GLOG_SUPPLY,
	GLOG_MISSION_MAP,
	GLOG_SEVEN_ITEM_ACTION,
	GLOG_MAIL,
	GLOG_CONSIGN,
	GLOG_MISSION_MAP_JOIN_INFO,
	GLOG_SOUL_CONSIGN,
	GLOG_CUBE,
	GLOG_USER_INFO,
	GLOG_TRADE,
	GLOG_CUSS_REPORT,
	GLOG_COIN_STATIC,
	GLOG_BAN_COIN,
	GLOG_BAN_REASON,
	GLOG_BILLING,
	GLOG_PCBANG,
	GLOG_SEVEN_ITEMACTION,
	GLOG_AUTO_QUESTION_AND_ANSWER,					// 오토 유저 질문과 답변
	GLOG_ITEM_STATUS,
	GLOG_AVATAR,
	GLOG_DAILY_MOB_DROP_COIN,
	GLOG_DAILY_PRODUCE,
	GLOG_PET,
	GLOG_DAILY_REGION_WAR_TAX,
	GLOG_DAILY_REGION_WAR_USE_ITEM,
	GLOG_DAILY_REGION_WAR_REVIVE,
	GLOG_DAILY_REGION_WAR_LOG,
	GLOG_ITEM_UPGRADE,
	GLOG_ITEM_CREATE_LIMIT,
	GLOG_TODAY_MISSION,
	GLOG_RAID_MOB,
	GLOG_RAID_MOB_ITEM,
	GLOG_DUNGEON_CLEAR_LOG,
	GLOG_EVENT_ITEM,
	GLOG_CASH_EVENT_ITEM,
	GLOG_GUILD_RESEARCH,
	GLOG_MISSION_TOURNAMENT_SEED_LOG,
	GLOG_MISSION_TOURNAMENT_LOG,
	GLOG_MISSION_MAP_RANKING_LOG,
	GLOG_MOB_DEAD_LOG,
	GLOG_WEAR_UPGRADE_LOG,
	GLOG_PK_COUNT_LOG,
	GLOG_PET_FOOD,
	GLOG_TRANSMIGRATE_BOOST_EVENT_SERVER,
	GLOG_GROWTH_WEAPON_RANKUP,
	GLOG_GROWTH_WEAPON_EXP,
	GLOG_ITEM_STATUS_EQUIPMENT,
	GLOG_ITEM_STATUS_NON_EQUIPMENT,
	GLOG_CONSIGN_BUY,
	GLOG_CONSIGN_REGIST,
	GLOG_TIMER_CASH,
	GLOG_CASH_AFFECT,
	GLOG_LUCKY_ROULETTE,
	GLOG_RESET_INSTANCE,
};

enum GLOG_SUPPLY_FLAG
{
	GLOG_SUPPLY_COIN_IN,							// 코인 들어옴
	GLOG_SUPPLY_COIN_OUT,							// 코인 나감
	GLOG_SUPPLY_ITEM_IN,							// 아이템 들어옴
	GLOG_SUPPLY_ITEM_OUT,							// 아이템 나감
	GLOG_SUPPLY_EXP_IN,								// 경험치 들어옴
	GLOG_SUPPLY_EXP_OUT,							// 경험치 나감
	GLOG_SUPPLY_SEVEN_IN,							// 7혼 들어옴
	GLOG_SUPPLY_SEVEN_OUT,							// 7혼 들어옴
};

/*
GLOG_ITEM_TYPE_EMPTY					없음		없음		없음				없음
GLOG_ITEM_TYPE_NORMAL,					bag_type	bag_index	없음				없음
GLOG_ITEM_TYPE_DUR_CHANGE,				bag_type	bag_index	old_dur				new_dur
GLOG_ITEM_TYPE_COUNT_CHANGE,			bag_type	bag_index	old_count			new_count
GLOG_ITEM_TYPE_DROP,					actor_sort	map_x		map_z				없음
GLOG_ITEM_TYPE_RANDOM_UPGRADE,			bag_type	bag_index	upgrade_index		new_upgrade_value
GLOG_ITEM_TYPE_RANDOM_UPGRADE_FAIL,		bag_type	bag_index	upgrade_index		old_upgrade_value
GLOG_ITEM_TYPE_UPGRADE,					bag_type	bag_index	old_upgrade			new_upgrade
*/

enum GLOG_ITEM_TYPE
{
	GLOG_ITEM_TYPE_EMPTY,							// value들이 없는 로그
	GLOG_ITEM_TYPE_NORMAL,							// 기본적인 아이템 로그 타입
	GLOG_ITEM_TYPE_DUR_CHANGE,						// 아이템 내구도 증감
	GLOG_ITEM_TYPE_COUNT_CHANGE,					// 아이템 갯수 증감
	GLOG_ITEM_TYPE_DROP,							// 아이템 드롭
	GLOG_ITEM_TYPE_RANDOM_UPGRADE,					// 렌덤 옵션 업그레이드 성공
	GLOG_ITEM_TYPE_RANDOM_UPGRADE_FAIL,				// 렌덤 옵션 업그레이드 실패
	GLOG_ITEM_TYPE_UPGRADE,							// 옵션 업그레이드
	GLOG_ITEM_TYPE_MAX,
};

/*
GLOG_SEVEN_ITEM_TYPE_NORMAL					bag_type	bag_index	없음				없음
GLOG_SEVEN_ITEM_TYPE_COUNT_CHANGE,			bag_type	bag_index	old_count			new_count
GLOG_SEVEN_ITEM_TYPE_UPGRADE,				bag_type	bag_index	old_upgrade			new_upgrade
*/

enum GLOG_SEVEN_ITEM_TYPE
{
	GLOG_SEVEN_ITEM_TYPE_NORMAL,					// 기본적인 아이템 로그 타입
	GLOG_SEVEN_ITEM_TYPE_COUNT_CHANGE,				// 아이템 갯수 증감
	GLOG_SEVEN_ITEM_TYPE_UPGRADE,
	GLOG_SEVEN_ITEM_TYPE_MAX,
};

inline void SEVEN_ITEM_ACTION_LOG(TOwnSoulItem* item, tINT actor_id, tINT bag_type, tINT bag_index, tINT log_time, tINT log_reason)
{
	Logger::instance()->deliver(GLOG_SEVEN_ITEMACTION, log_time,
		"%d,%d,%d,%lld,%d,%d,%d,%d",
		actor_id,
		bag_type,
		bag_index,
		item->getUid().getSerial(),
		item->getId(),
		item->getCount(),
		item->getUpgrade(),
		log_reason);
}

inline void SEVEN_ITEM_ACTION_LOG(TOwnSealItem* item, tINT actor_id, tINT bag_type, tINT bag_index, tINT log_time, tINT log_reason)
{
	Logger::instance()->deliver(GLOG_SEVEN_ITEMACTION, log_time,
		"%d,%d,%d,%lld,%d,%d,%d,%d",
		actor_id,
		bag_type,
		bag_index,
		item->getUid().getSerial(),
		item->getId(),
		0,
		0,
		log_reason);
}

enum
{
	ITEM_STATUS_FLAG_NONE,
	ITEM_STATUS_FLAG_CREATE,
	ITEM_STATUS_FLAG_ACTOR_IN,
	ITEM_STATUS_FLAG_ACTOR_OUT,
	ITEM_STATUS_FLAG_DELETE,
};
