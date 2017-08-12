#pragma once

#define ACTOR_GUILDTITLE_MIN			(4)
#define ACTOR_GUILDTITLE_MAX			(20 + 1)
#define ACTOR_GUILDTITLE_CREATE_MAX		(16 + 1)
#define ACTOR_GUILDMEMBER_MAX			(100)
#define ACTOR_GUILD_POSITION_NAME_MAX	(16 + 1)
#define ACTOR_GUILD_NOTIFY_MAX			(200 + 1)

#define GUILD_AFFECT_MAX				10
#define GUILD_RESEARCH_MAX				9

enum STATUS_GUILDSORT_
{
	STATUS_GUILDGRADE_EMPTY,
	STATUS_GUILDGRADE_1,
	STATUS_GUILDGRADE_2,
	STATUS_GUILDGRADE_3,
	STATUS_GUILDGRADE_4,
	STATUS_GUILDGRADE_5,
	STATUS_GUILDGRADE_6,
	STATUS_GUILDGRADE_7,
	STATUS_GUILDGRADE_8,
	STATUS_GUILDGRADE_9,
	STATUS_GUILDGRADE_10,
	STATUS_GUILDGRADE_MAX
};

enum {
	STATUS_GUILDPOSITION_NOT_MEMBER,		// 길드원이 아님
	STATUS_GUILDPOSITION_ONE_START,			// 1성직급
	STATUS_GUILDPOSITION_TWO_START,			// 2성직급
	STATUS_GUILDPOSITION_THREE_START,		// 3성직급
	STATUS_GUILDPOSITION_FOUR_START,		// 4성직급
	STATUS_GUILDPOSITION_MASTER,			// 길드마스터
	STATUS_GUILDPOSITION_GRADE_MAX,
};

#pragma pack(push, 4)
struct TGuildResearch
{
	tINT research_id_;
	tUINT start_time_;
	tINT research_time_;
	tINT research_activity_;
	tUINT completed_time_;
	tUINT use_expired_time_;
};

struct TGuildInfo
{
	guild_id_t	id_;										// 길드 고유 번호
	tBYTE		sort_;										// 길드 타입 (회,방...)
	tBYTE		grade_;										// 길드 등급 (1 ~ 5등급)
	tCHAR		name_[ACTOR_GUILDTITLE_MAX];				// 길드명
	tINT		member_limit_;								// 길드 소유 인원
	tINT		mark_;										// 문장 번호
	tCHAR		notify_message_[ACTOR_GUILD_NOTIFY_MAX];	// 길드 공지
	tINT		make_guild_item_;							// 길드가 소유한 장인산물
	tINT		battle_guild_item_;							// 길드가 소유한 전쟁산물
	tINT		purify_guild_item_;							// 길드가 소유한 정화산물
	tINT		income_rate_;								// 길드장 수입률
	tINT		keep_lines_;
	tINT		tournament_id_;							// 토너먼트 참가 여부 - 0: 참가 중 아님, 1: 참가 중
	TAffect		affect_[GUILD_AFFECT_MAX];					// 길드 어펙트
	tINT		riches_guild_item_;
	TGuildResearch research_[GUILD_RESEARCH_MAX];			// 길드 연구
};

struct TGuildMember
{
	tINT	actor_id_;						// 길드원의 actor id
	tCHAR	name_[STATUS_NAME_MAX];			// 길드원명
	tBYTE	guild_position_;				// 길드원 위치
	tBYTE	job_;							// 길드원 직업
	tINT	logonoff_;						// 길드원의 logon/off 상태, 0 - on, 0 > - logout
	level_t level_;							// 길드원 레벨
	tINT	donate_make_guild_item_;		// 길드원이 기부한 장인산물
	tINT	donate_battle_guild_item_;		// 길드원이 기부한 전쟁산물
	tINT	donate_purify_guild_item_;		// 길드원이 기부한 정화산물
	tINT	donate_riches_guild_item_;
};

struct TGuildAffect
{
	tINT affect_id_;
	tUINT expired_time_;
};

#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////
// 공성
enum SiegeStatus
{
	SIEGE_STATUS_NONE,
	SIEGE_STATUS_READY,
	SIEGE_STATUS_BATTLE,
};
