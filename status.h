#pragma once

#include <stdlib.h>
#include <string.h>
#include <string>

#include "rntype.h"
#include "boostcommon.h"
#include "define_quest.h"

//////////////////////////////////////////////////////////////////////////
// actor와 관련없는 define
#define MAKE_MAP_CHANNEL_ID(x, y)					((x) << 16 | (y))
#define SERVER_GROUP_ID_MAX							(32)
#define SERVER_MAP_ID_MAX							(128)
#define SERVER_CHANNNEL_ID_MAX						(32)
#define SERVER_STATUS_VALUE_MAX						3

inline tBOOL checkGroupMapChannelId(tUINT gid, tUINT mid, tUINT cid)
{
	return (gid < SERVER_GROUP_ID_MAX && mid < SERVER_MAP_ID_MAX && cid < SERVER_CHANNNEL_ID_MAX);
}


#define MAX_DICE	1000000

/// 켜짐
const tBOOL				cON			= 1;
/// 꺼짐
const tBOOL				cOFF		= 0;

#define UPGRADE_GRADE_MAX		(31)

#define LIMIT_INTMAX	(tINT)2000000000

#define BETSTATE_MAX 19
#define OLD_DICE_MAX 5

#define UniqueItemIdManager_START_TIME				(1259593200L)		// 2009-12-1 0:0:0

enum STATUS_MOVEMAP_
{
	STATUS_MOVEMAP_INMAP,			// in map
	STATUS_MOVEMAP_MOVEMAP,			// 맵이동
	STATUS_MOVEMAP_EVENT_MAP,		// 이벤트맵 아이템 사용이동
	STATUS_MOVEMAP_LANDMARK,		// 대지부 사용으로 이동
	STATUS_MOVEMAP_PORTALALLAREA,	// 축지첩 사용으로 이동
	STATUS_MOVEMAP_CHARM,			// 이동부적 사용으로 이동
};

//////////////////////////////////////////////////////////////////////////
// actor와 관련있는 define
typedef tINT64	exp_t;
typedef tINT64	jackpot_coin_t;
typedef tINT64	coin_t;
typedef tBYTE	level_t;
typedef tINT	fame_t;
typedef tINT	guild_id_t;
typedef tBYTE	job_t;
typedef tUINT	mail_regist_t;
typedef tINT64	cash_t;

#define CUSTOMIZE_DATA_BODY_COUNT	12
#define CUSTOMIZE_DATA_FACE_COUNT	26
#define CUSTOMIZE_DATA_MAX		(CUSTOMIZE_DATA_BODY_COUNT + CUSTOMIZE_DATA_FACE_COUNT)
#define USER_LEVEL_MAX	256

#define ACTOR_ID_EMPTY			0
#define ACTOR_HAVE_MAX			50		// 통합 수 최대 갯수 40 개
#define ORIGINAL_ACTOR_HAVE_MAX	10		// 통합 전 최대 갯수 10 개
#define ACTOR_WEAPON_HAVE_MAX	3
#define ACTOR_PARTY_MAX			4

#define STATUS_NAME_MAX			(16 + 1)
#define STATUS_NAME_MIN			2
#define STATUS_ITEM_EMPTY		0
#define STATUS_ITEMWEAR_MAX		25

#define STATUS_ITEMLIST_INIT_LINE	4		// 초기 가방 라인수
#define STATUS_ITEMLIST_LINE_MAX	11		// 최대 가방 라인수
#define STATUS_ITEMLIST_COLUMN_MAX	8
#define STATUS_ITEMLIST_MAX			(STATUS_ITEMLIST_LINE_MAX * STATUS_ITEMLIST_COLUMN_MAX)	// 72

#define STATUS_CASH_ITEMLIST_LINE		5		// 캐쉬 가방 라인수
#define STATUS_CASH_ITEMLIST_COLUMN_MAX	8
#define STATUS_CASH_ITEMLIST_MAX	(STATUS_CASH_ITEMLIST_LINE * STATUS_CASH_ITEMLIST_COLUMN_MAX)	// 40

#define STATUS_INVENLIST_MAX		( STATUS_ITEMLIST_MAX + STATUS_CASH_ITEMLIST_MAX ) // 72 + 40

#define STATUS_KEEPLIST_INIT_LINE	4		// 초기 창고 라인수
#define STATUS_KEEPLIST_LINE_MAX	9		// 최대 창고 라인수
#define STATUS_KEEPLIST_COLUMN_MAX	8
#define STATUS_KEEPLIST_MAX			(STATUS_KEEPLIST_LINE_MAX * STATUS_KEEPLIST_COLUMN_MAX)	// 72

#define STATUS_GUILDKEEPLIST_INIT_LINE	4	// 초기 문파 창고 라인수
#define STATUS_GUILDKEEPLIST_LINE_MAX	12	// 최대 문파 창고 라인수
#define STATUS_GUILDKEEPLIST_COLUMN_MAX	8
#define STATUS_GUILDKEEPLIST_MAX		(STATUS_GUILDKEEPLIST_LINE_MAX * STATUS_GUILDKEEPLIST_COLUMN_MAX)	// 72

#define STATUS_ACTOR_PET_INIT_LINE	4		// 초기 팻 창고 라인 수
#define STATUS_ACTOR_PET_LINE_MAX	3		// 최대 팻 창고 라인 수
#define STATUS_ACTOR_PET_COLUMN_MAX	8		// 최대 팻 창고 컬럼 수
#define STATUS_ACTOR_PET_INVENLIST_MAX	( STATUS_ACTOR_PET_LINE_MAX * STATUS_ACTOR_PET_COLUMN_MAX )			// 24

#define STATUS_ACTOR_PET_LIST_MAX	16		// 최대 소유할수 있는 팻의 갯수

#define STATUS_AFFECT_MAX			50
#define STATUS_AFFECT_POSITIVE_MAX	28
#define STATUS_AFFECT_NEGATIVE_MAX	40

#define STATUS_AFFECT_NONECASH_MAX	40
#define STATUS_AFFECT_CASH_MAX		10


#define STATUS_FUNCLINK_PER_ROW		10
#define STATUS_FUNCLINK_PER_SET		20
#define STATUS_FUNCLINK_TOTAL_SET	3
#define STATUS_FUNCLINK_MAX			(STATUS_FUNCLINK_PER_SET * STATUS_FUNCLINK_TOTAL_SET)

#define STATUS_SKILL_EMPTY		0
#define STATUS_USER_SKILL_MAX	70
#define STATUS_MOB_SKILL_MAX	10
#define STATUS_SKILL_MAX		(STATUS_USER_SKILL_MAX + STATUS_MOB_SKILL_MAX)

#define STATUS_SKILL_ITEM_MAX	20

// friend
#define STATUS_ACCEPT_FRIEND_MAX	30
#define STATUS_DENY_FRIEND_MAX	30

#define ITEM_NAME_MAX			50 + 1
#define SEARCH_ITEM_LIST_MAX	8

#define STATUS_STR_MAX					( 255 )			// 힘 최대치
#define STATUS_DEX_MAX					( 255 )			// 재주 최대치
#define STATUS_VITAL_MAX				( 255 )			// 체력 최대치
#define STATUS_INTE_MAX					( 255 )			// 지능 최대치
#define STATUS_AGIL_MAX					( 255 )			// 민첩 최대치

#define STATUS_HP_MAX					( 2147483647 )		// 외공 최대치 SHRT_MAX
#define STATUS_MP_MAX					( 32767 )		// 내공 최대치
#define STATUS_SP_MAX					( 32767 )		// 활력 최대치

#define ACTIVITY_POINT_MAX	2000
#define ACTIVITY_POINT_MIN	0

#define MAX_KEY_BINDING			2		// 한 동작당 설정 가능한 키 수
#define MAX_KEY_BINDING_LIST	80		// 설정 가능한 동작 수

#define MAX_TODAY_MISSION_COUNT	10	// Today Mission 최대 개수

#define COMBINE_KEEP_COUNT		4		// 서버 통합 창고 갯수


//XX 아이템의 상태값
enum STATUS_ITEMSTATE_
{
	STATUS_ITEMSTATE_NORMALITEM = 0,	// 0 보통 아이템
	STATUS_ITEMSTATE_TOOLITEM = 1,		// 1 툴 아이템
	STATUS_ITEMSTATE_TIMERITEM = 2,		// 2 타이머 아이템
};

enum STATUS_ITEM_BIND_STATE_
{
	STATUS_ITEM_BIND_NONE		= 0,	// 종속 안됨
	STATUS_ITEM_BIND			= 1,	// 종속 됨
	STATUS_ITEM_SEAL_BIND		= 2,	// 봉인된 종속 아이템
};

enum USER_JOB_
{
	USER_JOB_NONE,
	USER_JOB_WARRIOR,	// 관인
	USER_JOB_ASSASSIN,	// 영인
	USER_JOB_WIZARD,	// 월인
	USER_JOB_MONK,		// 승려 (현재 사용안함)
	USER_JOB_MAX
};

enum FORMSPECIAL_BSPECIAL_
{
	FORMSPECIAL_BSPECIAL_PKCOUNT,			// 0 Chao  수치
	FORMSPECIAL_BSPECIAL_PKMODE,			// 1 자동 PK 모드 ( 0 - 수동, 1 - 자동 )
	FORMSPECIAL_BSPECIAL_BODYSIZE,			// 2 체형
	FORMSPECIAL_BSPECIAL_GMGRADE,			// 6 GM 등급 - 0 일 경우 GM아님
	FORMSPECIAL_BSPECIAL_GMSTATUS,			// 7 GM 상태 - 0 일 경우 보통
	FORMSPECIAL_BSPECIAL_WEAR_HIDE,			// 8 0x01 - 투구 0x02 - 외형변경 0x04 - 아바타외형
	FORMSPECIAL_BSPECIAL_INVEN_LINE,		// 9 행낭 확장
	FORMSPECIAL_BSPECIAL_PARTY_AUTO_DIVIDE,	// 11 파티 자동 분배 0 - 수동, 1 - 자동
	FORMSPECIAL_BSPECIAL_JACKPOT_GIFT_TYPE,	// 12 잭팟 보상 타입 (bit), 0x01 - 게임 머니, 0x02 - 경험치, 0x04 - 버프, 0x08 - 아이템
	FORMSPECIAL_BSPECIAL_TOURNAMENT_ICON_VIEW,		// 토너먼트 아이콘 보여주기
	FORMSPECIAL_BSPECIAL_LINK_SET_NUM,		// 클라가 선택한 퀵슬롯번호
	FORMSPECIAL_BSPECIAL_DUEL_POINT,		// 비무 점수 (일일 획득 점수)
	FORMSPECIAL_BSPECIAL_ACTIVATOR_SEALED,	// 엑티베이터봉인유무
	FORMSPECIAL_BSPECIAL_EYE_COLOR,			// 눈동자 색깔
	FORMSPECIAL_BSPECIAL_LUCKY_LOULETTE_HAVE_COUNT_PER_DAY,	// 하루에 얻은 행운의 룰렛 수(0 - 5)
	FORMSPECIAL_BSPECIAL_LUCKY_LOULETTE_USED_COUNT_PER_DAY,	// 하루에 사용한 행운의 룰렛 수(0 - 5)

	FORMSPECIAL_BSPECIAL_PARTY_COIN_DIVIDE,	// 17 파티 금전 분배 0 - 자유, 1 - 공헌도, 2 - 균등
	FORMSPECIAL_BSPECIAL_EUP_ONOFF,			// eup type-1 아이템 on/off 여부

	FORMSPECIAL_BSPECIALMAX					// MAX
};

enum FORMSPECIAL_FSPECIAL_
{
	FORMSPECIAL_FSPECIAL_GM_MOVE_SPEED,
	FORMSPECIAL_FSPECIALMAX = 1,	// MAX
};

enum FORMSPECIAL_ISPECIAL_
{
	FORMSPECIAL_ISPECIAL_JACKPOT_POINT,				// 0 잭팟 포인트
	FORMSPECIAL_ISPECIAL_REGRET_COUNT,				// 참회 수치
	FORMSPECIAL_ISPECIAL_REVIVE_TIME,				// 부활 대기 시간
	FORMSPECIAL_ISPECIAL_EXPIRE_TIME_REVIVE_END,	// 부활 할수 있는 시간
	FORMSPECIAL_ISPECIAL_EXPIRE_TIME_REVIVE_DELAY,	// 부활 유예 시간
	FORMSPECIAL_ISPECIAL_DUEL_POINT,				// 비무 점수 (전체 획득 점수)
	XXX_FORMSPECIAL_ISPECIAL_SOUL_COLLECTOR_ID,		// 소울컬렉터ID
	XXX_FORMSPECIAL_ISPECIAL_SOUL_ENERGY,			// 혼수집량

	FORMSPECIAL_ISPECIAL_7SOUL_EVENT_ITEM,			// CBT 이벤트아이템 습득시 증가

	FORMSPECIAL_ISPECIAL_RESTORATION_EXP,			// XXX 사용안함. level_.restoraion_exp로 64bits로 변경함.

	FORMSPECIAL_ISPECIAL_REST_BONUS_EXP,			// 휴식 보너스
	FORMSPECIAL_ISPECIAL_COOLTIME_TELEPORT_SAVED,	// 저장위치 이동 쿨타임

	FORMSPECIAL_ISPECIAL_CHAT_BAN_LIMIT_TIME,		// 채팅 금지 남은 시간

	FORMSPECIAL_ISPECIAL_CASH_REMOTE_KEEP,				// 원격 창고 호출
	FORMSPECIAL_ISPECIAL_CASH_DAETH_PENALTY_1_PERCENT,	// 죽음 패널피 감소 (경험치 감소 1% 고정)
	FORMSPECIAL_ISPECIAL_CASH_ITEM_PROTECT,				// 장비 보호 케쉬아이템 사용
	FORMSPECIAL_ISPECIAL_ENVIRMENT_OPTION,			// 환경 설정 옵션 + 퀵슬롯 Lock
	FORMSPECIAL_ISPECIAL_LIMIT_GUILD,				// 길드 생성, 가입 제한 시간 (unix time)
	FORMSPECIAL_ISPECIAL_DAILY_QUEST_UPDATE_TIME,		// 일일 반복 퀘스트 업데이트 시간
	FORMSPECIAL_ISPECIAL_CASH_REFUND_LIMIT,			// 환불 제한 시간(unix time)

	FORMSPECIAL_ISPECIAL_TOWER_EVENT_POINT,			// 사신도 이벤트 포인트

	FORMSPECIAL_ISPECIAL_LUCKY_LOULETTE_GAGE_SEC,	// 다음 룰렛 수를 얻기 까지의 시간(sec)
	FORMSPECIAL_ISPECIAL_LUCKY_LOULETTE_TIME,		// 마지막으로 룰렛을 획득한 시간(unix time)
	FORMSPECIAL_ISPECIAL_DAILY_TOWER_UPDATE_TIME,	// 사신도 입장 횟수 업데이트 시간

	FORMSPECIAL_ISPECIAL_ONEDAY_QUEST_RESET_TIME,	// OneDay Quest Reset 시간

	FORMSPECIAL_ISPECIAL_CHAT_OPTION,				// 채팅 메시지 보는 필터링 옵션
	FORMSPECIAL_ISPECIAL_TODAY_MISSION_RESET_TIME,	// Today Mission Reset 시간
	FORMSPECIAL_ISPECIAL_CASH_PROTECT_PK_PENALTY,	// pk 패널티 방지
	FORMSPECIAL_ISPECIAL_CASH_UNSEAL_SOUL_SLOT,		// 7혼 슬롯 봉인 해제

	FORMSPECIAL_ISPECIAL_PROTECT_BLESS_HP_REMAIN,	// 수호의 가호 버프에 의해 적용된 HP 값의 남은 양.
	FORMSPECIAL_ISPECIAL_EUP_REMAIN_TIME,			// eup type-1 아이템 남은 시간
	FORMSPECIAL_ISPECIAL_EUP_USE_TIME,				// eup type-1 아이템 종료 시간
	FORMSPECIAL_ISPECIAL_EUP_TYPE_2_EXPIRE_TIME,	// eup type-2 아이템 종료 시간

	FORMSPECIAL_ISPECIAL_DAILY_INS_RESET_COUNT_TIME,	// 인던 쿨타임 리셋 횟수 업데이트 시간

	FORMSPECIAL_ISPECIAL_PICKUP_ITEM_AUTO,			// 자동 줍기 expire time

	FORMSPECIAL_ISPECIALMAX,						// MAX
};

#define SPECIAL_ABILITY_COUNT_MAX		( 19 )
#define SPECIAL_ABILITY_EXP_MAX			( 100 )


// 칠혼 속성
enum
{
	SEVEN_SOUL_SORT_NONE = 0,
	SEVEN_SOUL_SORT_EARTH,			// 땅
	SEVEN_SOUL_SORT_FIRE,			// 불
	SEVEN_SOUL_SORT_TREE,			// 나무
	SEVEN_SOUL_SORT_WATER,			// 물
	SEVEN_SOUL_SORT_METAL,			// 금속
	SEVEN_SOUL_SORT_WIND,			// 바람
	SEVEN_SOUL_SORT_LIFE,			// 생명
	SEVEN_SOUL_SORT_MAX,
};

#define SEVENSOUL_EPISODE_MAX				1

#define SEVENSOUL_WEAR_ITEM_MAX				(7)
#define SEVENSOUL_INVEN_ITEM_MAX			(100)	// 가방에 저장되는 7혼지서 아이템 최대치
#define SEVENSOUL_UNREGIST_ITEM_MAX			(6)		// 등록 안한 7혼지서

#define SEVENSEAL_FORCE_GIVE_ITEM_MAX		(3)		// 강제 지급되는 봉인지서 최대치	sort 2
#define SEVENSEAL_NORMAL_GIVE_ITEM_MAX		(1)		// 일반 지급되는 봉인지서 최대치	sort 1
#define SEVENSEAL_EXCHANGE_ITEM_MAX			(3)		// 교환 봉인지서 최대치	sort 3
#define SEVENSEAL_ITEM_MAX					( SEVENSEAL_FORCE_GIVE_ITEM_MAX + SEVENSEAL_EXCHANGE_ITEM_MAX )		// 봉인지서 저장 최대치

#define RANDOM_OPTION_DATA_MAX	(8)
#define ITEM_ABILITY_COUNT		(7)

typedef std::vector<tSINT>	completed_quest_vec_t;
typedef std::map<tUINT, tINT> limit_exchange_map_t;

#define AUTO_PROTECT_WORD_MAX	( 8 + 1 )

#pragma pack(push, 4)

union TUniqueItemId
{
	tUINT64	serial_;			// 고유의 64bit 수
	struct
	{
		tUINT64	gid_		: 5;	// 생성된 GROUP ID, 0 ~ 31
		tUINT64	map_id_		: 7;	// 생성된 MAP ID, 0 ~ 127
		tUINT64 channel_id_ : 5;	// 생성된 CHANNEL ID, 0 ~ 31
		tUINT64	sort_		: 6;	// 생성 타입, 0 ~ 63
		tUINT64	index_		: 11;	// 생성된 순서, 0 ~ 2047
		tUINT64	time_		: 30;	// 생성 시간 (unix time)
	} data;

	tUINT64 getSerial()			{ return serial_; }
	void setSerial(tUINT64 s)	{ serial_ = s; }
	tUINT getTime()				{ return data.time_ + UniqueItemIdManager_START_TIME; }
	tUINT getSort()				{ return data.sort_; }
};

struct TOwnItemBase
{
private:
	struct
	{
		tUINT		id_		: 24;
		tUINT		color_	: 5;
		tUINT		state_	: 2;
	} item_;
	tINT			count_;
	struct
	{
		tBYTE		upgrade_		: 5;
		tBYTE		bind_			: 2;
		tBYTE		random_expand_	: 1;
	} flags_;
	struct
	{
		tBYTE		basic_		: 3;
		tBYTE		random_		: 3;
		tBYTE		avatar_		: 2;

	} op_count_;
	tBYTE			option_[RANDOM_OPTION_DATA_MAX];
	tBYTE			dur_;

public:
	tBYTE			getColor()						{ return item_.color_; }
	void			setColor(tBYTE color)			{ item_.color_ = color; }
	tBYTE			getState()						{ return item_.state_; }
	void			setState(tBYTE state)			{ item_.state_ = state; }
	tINT			getId() const					{ return item_.id_; }
	void			setId(tINT id)					{ item_.id_ = id; }

	tINT			getCount() const 				{ return count_; }
	void			setCount(tINT c)				{ count_ = c; }

	tUINT			getExpiredTime()				{ return (item_.state_ == STATUS_ITEMSTATE_TIMERITEM) ? (tUINT)count_ : 0; }

	tINT			getUpgrade()					{ return flags_.upgrade_; }
	void			setUpgrade(tINT upgrade)		{ flags_.upgrade_ = upgrade; }
	tBYTE			getBind()						{ return flags_.bind_; }
	tBOOL			isBind()						{ return ( flags_.bind_ == 1 ) ? cTRUE : cFALSE; }
	tBOOL			isBindSeal()					{ return ( flags_.bind_ == 2 ) ? cTRUE : cFALSE; }
	void			setBind(tINT val)				{ flags_.bind_ = val; }
	void			sealBind()						{ flags_.bind_ = STATUS_ITEM_SEAL_BIND; }
	tINT			getBasicCount()					{ return op_count_.basic_; }
	void			setBasicCount(tINT count)		{ op_count_.basic_ = count; }
	tINT			getRandomCount()				{ return op_count_.random_; }
	void			setRandomCount(tINT count)		{ op_count_.random_ = count; }
	tINT			getAvatarCount()				{ return op_count_.avatar_; }
	void			setAvatarCount(tINT count)		{ op_count_.avatar_ = count; }
	tINT			getOption(tINT index)			{ return option_[index]; }
	void			setOption(tINT index, tINT val)	{ option_[index] = val; }
	void			resetOption()					{ memset( option_, 0x00, sizeof(option_) ); }
	tINT			getTotalOptionCount()			{ return op_count_.basic_ + op_count_.random_ + op_count_.avatar_; }

	tINT			getDur()						{ return dur_; }
	void			setDur(tBYTE dur)				{ dur_ = dur; }
	void			decDur()						{ if( dur_ > 0 ) dur_--; }

	tBYTE*			getOptionPtr()					{ return option_; }

	void			setRandomExpand(tINT flag)		{ flags_.random_expand_ = flag; }
	tINT			getRandomExpand()				{ return flags_.random_expand_; }
};

struct TOwnItem : public TOwnItemBase
{
	TUniqueItemId	uid_;		// 아이템 고유 번호

	TUniqueItemId&	getUid()						{ return uid_; }
};


struct TOwnSkill
{
	tINT	id_;						// Skill ID
	tSINT	remain_cool_time_;			// remain individual cool time
	tSINT	remain_group_cool_time_;	// remain group cool time

	tINT	getID()				{ return id_; }
	void	setID( tINT id )	{ id_ = id;   }

	tSINT	getCoolTime()					{ return remain_cool_time_; }
	void	setCoolTime( tSINT time )		{ remain_cool_time_ = time; }

	tSINT	getGroupCoolTime()				{ return remain_group_cool_time_; }
	void	setGroupCoolTime( tSINT time )	{ remain_group_cool_time_ = time; }
};

struct TAffect
{
	tUSINT	affect_id_;
	tINT	remain_time_;
};

struct TFunctionLink
{
	tUINT	type_		: 8;		// gameshare.h에 정의됨 (44 lines)
	tUINT	listnum_	: 24;
};

// 캐릭터 리스트 보여줄 때 필요한 정보
struct TActorListData
{
	tCHAR name_[STATUS_NAME_MAX];
	tBYTE face_;
	tBYTE hair_;
	tBYTE hair_color_;
	tBYTE skin_color_;
	tBYTE job_;
	level_t level_;
	tINT map_id_;
	tFLOAT x_;
	tFLOAT z_;

	tCHAR customize_data_[CUSTOMIZE_DATA_MAX];
	TOwnItem wear_[STATUS_ITEMWEAR_MAX];

	tBYTE guild_position_;

	tBYTE delete_flag_;
	tUINT delete_time_;
	tBYTE wear_flag_;
	tINT booster_event_group_id_;
};

//actor 표현을 위한 것들이 모임
struct TFormStatus
{
	tCHAR		name_[STATUS_NAME_MAX];				// 이름
	tBYTE		kind_;								// 성향, 0 - 중, 1 - 정, 2 - 사
	tINT		fame_;								// 명성
	tBYTE		face_;								// 얼굴
	tBYTE		hair_;								// 머리
	tBYTE		hair_color_;						// 머리색
	tBYTE		skin_color_;						// 피부색
	tBYTE		job_;								// 직업
	guild_id_t	guild_id_;							// 고유번호
	tBYTE		guild_position_;					// 문파에서의 위치
	tBYTE		level_;								// 공력

	tINT		current_gp_;						// 현재 GP 근성(guts) 수치
	tINT		current_hp_;						// 현재 HP
	tINT		current_mp_;						// 현재 MP

	tINT		map_id_;							// 현재 지역
	tFLOAT		x_;									// 현재 위치
	tFLOAT		z_;									//    "
	tFLOAT		direction_;							// 방향
	TOwnItem	wear_[STATUS_ITEMWEAR_MAX];			// 장착
	tUINT		wear_upgrade_[STATUS_ITEMWEAR_MAX];	// 장착창 업그레이드 유무
	TAffect		affect_[STATUS_AFFECT_MAX];			// 상태이상
	tBYTE		bspecial_[FORMSPECIAL_BSPECIALMAX];	// 특수값1
	tFLOAT		fspecial_[FORMSPECIAL_FSPECIALMAX];	// 특수값2
	tINT		ispecial_[FORMSPECIAL_ISPECIALMAX];	// 특수값3
	tCHAR		customize_data_[CUSTOMIZE_DATA_MAX];	// 캐릭터 커스터마이즈 데이터

	tINT		saved_position_map_id_;					// 저장 위치
	tFLOAT		saved_position_x_;
	tFLOAT		saved_position_z_;

	tINT		activity_point_;

	tINT		landmark_position_map_id_;					// 저장 위치
	tFLOAT		landmark_position_x_;
	tFLOAT		landmark_position_z_;

	tINT		tower_count_;
	tINT		pcbang_tower_count_;
	tINT		ins_reset_count_;

//none save
	tBYTE		xxx_mode_;
	tINT		mind_skill_id_;
	tINT		dash_skill_id_;
	tINT		guard_skill_id_;
	tINT		hidden_skill_id_;
	tINT		stance_skill_id_;

	tINT		getCurrentGP()				{ return current_gp_; }
	void		setCurrentGP(tINT point)	{ current_gp_ = (point > 0) ? point : 0; }
	void		incCurrentGP(tINT point)	{ current_gp_ += point; }
	void		decCurrentGP(tINT point)	{ current_gp_ -= point; }

	tINT		getCurrentHP()				{ return current_hp_; }
	void		setCurrentHP(tINT point)	{ current_hp_ = (point > 0) ? point : 0; }
	void		incCurrentHP(tINT point)	{ current_hp_ += point; }
	void		decCurrentHP(tINT point)	{ current_hp_ -= point; }

	tINT		getCurrentMP()				{ return current_mp_; }
	void		setCurrentMP(tINT point)	{ current_mp_  = (point > 0) ? point : 0; }
	void		incCurrentMP(tINT point)	{ current_mp_ += point; }
	void		decCurrentMP(tINT point)	{ current_mp_ -= point; }

	tINT		getMindSkillID()			{ return mind_skill_id_; }
	void		setMindSkillID(tINT id)		{ mind_skill_id_ = id; }
	void		resetMindSkillID()			{ mind_skill_id_ = 0; }

	tINT		getDashSkillID()			{ return dash_skill_id_; }
	void		setDashSkillID(tINT id)		{ dash_skill_id_ = id; }
	void		resetDashSkillID()			{ dash_skill_id_ = 0; }

	tINT		getGuardSkillID()			{ return guard_skill_id_; }
	void		setGuardSkillID(tINT id)	{ guard_skill_id_ = id; }
	void		resetGuardSkillID()			{ guard_skill_id_ = 0; }

	tINT		getHiddenSkillID()			{ return hidden_skill_id_; }
	void		setHiddenSkillID(tINT id)	{ hidden_skill_id_ = id; }
	void		resetHiddenSkillID()		{ hidden_skill_id_ = 0; }

	tINT		getStanceSkillID()			{ return stance_skill_id_; }
	void		setStanceSkillID(tINT id)	{ stance_skill_id_ = id; }
	void		resetStanceSkillID()		{ stance_skill_id_ = 0; }

	tUINT		getWearUpgradeTime( tINT array_index ) { return wear_upgrade_[array_index]; }
	void		setWearUpgradeTime( tINT array_index, tUINT expire_time ) { wear_upgrade_[array_index] = expire_time; }
};

//level up 관련된 것들이 모임
struct TLevelStatus
{
	exp_t	exp_;					// 현재 경험치
	exp_t	restoration_exp_;		// 복구 가능 경험치


	tSINT	str_;					// 힘
	tSINT	dex_;					// 재주
	tSINT	con_;					// 체력
	tSINT	int_;					// 지능
	tSINT	agi_;					// 민첩

	tSINT	current_sp_;				// 현재 활력

	tINT	xxx_event_casino_point_;	// 이벤트 카지노 횟수
	tINT	xxx_event_play_point_;		// 이벤트 접속시간
	tSINT	body_point_;			// 현묘
	tSINT	skill_point_;			// 스킬 포인트
	tSINT	awake_skill_point_;
	tINT	xxx_guild_point_;			// 길드 기여도
	tINT	mark_exp_;				// 훈장 경험치

	tBYTE	growth_wp_rank_;
	tINT	growth_wp_exp_;
	tINT	growth_wp_fat_count_;
	tUINT	growth_wp_last_fat_time_;


	//////////////////////////////////////////////////////////////////////////
	tSINT	getStr()	{	return str_;	}
	tSINT	getDex()	{	return dex_;	}
	tSINT	getCon()	{	return con_;	}
	tSINT	getInt()	{	return int_;	}
	tSINT	getAgi()	{	return agi_;	}

	void	setStr(tSINT point)	{	str_  = point;	}
	void	setDex(tSINT point)	{	dex_  = point;	}
	void	setCon(tSINT point)	{	con_  = point;	}
	void	setInt(tSINT point)	{	int_  = point;	}
	void	setAgi(tSINT point)	{	agi_  = point;	}

	tSINT	getBodyPoint()	{	return body_point_;	}
	void	setBodyPoint(tSINT point)	{	body_point_  = point;	}
	void	decBodyPoint(tSINT point)	{	body_point_ -= point;	}

	tSINT	getCurrentSP()				{	return current_sp_;		}
	void	setCurrentSP(tSINT	point)	{	current_sp_  = point;	}
	void	incCurrentSP(tSINT	point)	{	current_sp_ += point;	}
	void	decCurrentSP(tSINT	point)	{	current_sp_ -= point;	}
};

struct TOwnSealItem
{
	TUniqueItemId	uid_;		// 아이템 고유 번호
	tSINT			id_;

	TUniqueItemId&	getUid()						{ return uid_; }
	tSINT			getId()							{ return id_; }
	void			setId( tSINT id )				{ id_ = id; }
};

struct TOwnSoulItem
{
	TUniqueItemId	uid_;		// 아이템 고유 번호
	tINT			id_;
	tSINT			count_;
	tBYTE			upgrade_;

	TUniqueItemId&	getUid()						{ return uid_; }
	tINT			getId()							{ return id_; }
	void			setId( tINT id )				{ id_ = id; }
	tSINT			getCount()						{ return count_; }
	void			setCount( tSINT count )			{ count_ = count; }
	tBYTE			getUpgrade()					{ return upgrade_; }
	void			setUpgrade( tBYTE upgrade )		{ upgrade_ = upgrade; }
};

//분류에 관계 없이 actor에게 필요한 것들이 모임
struct TBasicStatus
{
	tINT		casino_point_;						// 겜블포인트(주사위 포인트)
	tINT		booster_event_group_id_;			// 부스트 이벤트 서버 이동한 서버 그룹 아이디
};

//item 관련된 것들이 모임
struct TItemStatus
{
	TOwnItem		list_[STATUS_INVENLIST_MAX];				// 아이템
	coin_t			coin_;									// 소유한 사이버머니
	TFunctionLink	link_[STATUS_FUNCLINK_MAX];				// 퀵 슬롯
	tUSINT			cool_time_[STATUS_SKILL_ITEM_MAX];		// 스킬 아이템 쿨 타임
};

//skill 관련된 것들이 모임
struct TTechStatus
{
	TOwnSkill	skill_[STATUS_SKILL_MAX];	// 전술 무공
};


// 진행 중인 퀘스트
struct TActiveQuestStatus
{
	tSINT	id_;						// 퀘스트 아이디
	tSINT	step_;						// 퀘스트 단계
	tBYTE	flags_[QUEST_FLAG_MAX];		// 퀘스트 진행 상태
	tBYTE	group_flags_;				// 퀘스트 진행 상태
	tUINT	expire_time_;				// 만료 시간
	BYTE	selected_;					// 선택된 퀘스트
};

// 일일 퀘스트 그룹
struct TDailyQuestStatus
{
	tBYTE	count_;
};

// 거간꾼 퀘스트
struct TBrokerQuestStatus
{
	tUINT	quest_id_;						// 퀘스트 아이디
	tSINT	step_;							// 퀘스트 단계

	tUINT	prize_exp_;						// 보상 경험치
	tUINT	prize_coin_;					// 보상 금전
	tUINT	prize_jackpot_;					// 보상 잭팟 포인트

	tUINT	quest_mission_flag_value[QUEST_BROKER_FLAG_MAX];		// 거간꾼 퀘스트 소임 수치 필요값
	tUINT	cur_quest_mission_flag[QUEST_BROKER_FLAG_MAX];			// 거간꾼 퀘스트 소임 수치 현재값

	tUINT	quest_mission_item_grade_value[QUEST_BROKER_FLAG_MAX];	// 거간꾼 퀘스트 소임 필요 아이템 강화 수치

	tUINT	quest_random_id[QUEST_BROKER_FLAG_MAX];					// 거간꾼 퀘스트 소임 필요 아이디

	tUINT	expire_time_;					// 만료 시간

	BYTE	accepted_;						// 퀘스트 수락 여부

	BYTE	selected_;						// 퀘스트 Select State
};

// 퀘스트 관련
struct TQuestStatus
{
	TActiveQuestStatus		active_quest_[MAX_QUEST];				// 진행중인퀘스트
	TDailyQuestStatus		daily_quest_[QUEST_DAILY_GROUP_MAX];	// 일일 반복퀘스트 그룹
	tINT					karma_step_;							// 현재진행중인카르마단계
	TBrokerQuestStatus		broker_quest_[QUEST_BROKER_OWN_MAX];	// 거간꾼 퀘스트
};

// 전문 능력
struct TSpecialAbility
{
	level_t	level_;
	tBYTE	exp_;
};

struct TSpecialAbilityStatus
{
	TSpecialAbility	ability_[SPECIAL_ABILITY_COUNT_MAX];
};

// 칠혼 시스템
struct TSevenSoul
{
	tUINT64			soul_item_history_[SEVENSOUL_EPISODE_MAX];
	TOwnSealItem	seal_item_[SEVENSEAL_ITEM_MAX];
	TOwnSoulItem	unregist_item_[SEVENSOUL_UNREGIST_ITEM_MAX];
	TOwnSoulItem	inven_item_[SEVENSOUL_INVEN_ITEM_MAX];
	TOwnSoulItem	wear_item_[SEVENSOUL_WEAR_ITEM_MAX];
	tSINT			item_cool_time_[SEVENSOUL_WEAR_ITEM_MAX];
};

#define MAX_COMEBACK_EVENT_COUNT 30
struct TComeBackEventInfo
{
	tINT start_date_;
	tINT end_date_;
	tBOOL day_flag_[MAX_COMEBACK_EVENT_COUNT];
};

struct TOwnPet
{
	tINT			pet_id_;
	tINT			cool_time_;
	tINT			death_cool_time_;
	tINT			current_hp_;						// 현재 HP
	tINT			current_mp_;						// 현재 MP
	tINT			expire_time_;						// 펫 만료시간

	void			setPetId( tINT pet_id ) { pet_id_ = pet_id; }
	void			setCoolTime( tINT cool_time ) { cool_time_ = cool_time; }
	void			setDeathCoolTime( tINT death_cool_time ) { death_cool_time_ = death_cool_time; }
	void			setCurrentHp( tINT current_hp ) { current_hp_ = current_hp; }
	void			setCurrentMp( tINT current_mp ) { current_mp_ = current_mp; }
	void			setExpireTime( tINT expire_time ) { expire_time_ = expire_time; }
	tINT			getPetId() { return pet_id_; }
	tINT			getCoolTime() { return cool_time_; }
	tINT			getDeathCoolTime() { return death_cool_time_; }
	tINT			getCurrentHp() { return current_hp_; }
	tINT			getCurrentMp() { return current_mp_; }
	tINT			getExpireTime() { return expire_time_; }
};

struct TPetStatus
{
	TOwnPet			pet_list_[STATUS_ACTOR_PET_LIST_MAX];
//	TOwnItem		pet_inven_[STATUS_ACTOR_PET_INVENLIST_MAX];	// 팻 인벤
};

struct TTodayMissionStatus
{
	tBYTE mission_level_;									// Today Mission 을 받았을때의 Level
	tINT cur_value_[MAX_TODAY_MISSION_COUNT];				// Today Mission의 현재 Value
};

struct TStatus
{
	TFormStatus		form_;
	TLevelStatus	level_;
	TBasicStatus	basic_;
	TItemStatus		item_;
	TTechStatus		tech_;
	TQuestStatus	quest_;
	TSpecialAbilityStatus	speability_;
	TSevenSoul		sevensoul_;
	TComeBackEventInfo comeback_event_;
	TPetStatus		pet_;
	TTodayMissionStatus today_mission_;		// 오늘의 목표 이벤트

	tBYTE			delete_flag_;
	tUINT			logout_time_;			// 마지막 로그아웃 시간
	tBOOL			anniversary_event_;		// 일주년 이벤트 적용 플레그
	tBOOL			buff_event_;			// 버프 이벤트 적용 플레그
	tINT			daily_check_event_rcv_time_;	// 일일 체크 이벤트 아이템 받은 시간.
};

struct TStore
{
	TStatus	status_;
};

struct TShare
{
	tINT	seal_cool_time_;

	tUINT	auto_ip_;
	tUINT	auto_device_;
	tUINT	auto_process_;
	tUINT	auto_macro_;
	tUINT	auto_question_no_reply_;		// 질문에 응답을 하지 못한 횟수
	tUINT	auto_user_count_;				// 오토범이된 횟수
	tUINT	auto_user_expire_time_;			// 오토범 해제 시간

	tUINT	daily_pcbang_event_play_time_;	// 일일 피씨방 이벤트 누적 플레이 시각
	tUINT	daily_pcbang_event_update_time_;// 일일 피씨방 이벤트 업데이트 시각
	tBOOL	daily_pcbang_evnet_send_item_;	// 일일 피씨방 이벤트 아이템 받았는지 유무

	tUINT	pcbang_play_time_;				// 피씨방 플레이 시간
	tUINT	pcbang_last_reward_time_;		// 아이템 보상 받은 시간
	tINT	event_point_;					// 계정 이벤트 포인트

	tUINT	play_time_;						// 플레이 시간
	tUINT	last_reward_time_;				// 아이템 보상 받은 시간

	tUSINT	key_binding_[MAX_KEY_BINDING][MAX_KEY_BINDING_LIST];
	tINT	cash_event_shop_id_1400026_;			// 케쉬 이벤트 아이템 1400026
	tINT	cash_event_shop_id_1400027_;			// 케쉬 이벤트 아이템 1400027

	tBYTE	poll_choice1_;				// 투표 이벤트 선택1
	tBYTE	poll_choice2_;				// 투표 이벤트 선택2
};

//////////////////////////////////////////////////////////////////////////
struct TGroupStatus
{
	tSINT		gid_;
	tSINT		status_;			// 서버군 상태
	tBYTE		population_;		// 1 ~ 4. 1: 원할, 2: 보통, 3: 혼잡, 4: 매우 혼잡
	tBYTE		create_character_flag_;		// 캐릭터를 생성할 수 있는 서버군인가?
};

struct TDatabaseInfo
{
	std::string	host_;
	std::string	port_;
	std::string	dbname_;
	std::string	id_;
	std::string	password_;
};

#define MAX_WORLD_EVENT_FLAGS	5
struct WorldEventTable
{
	tINT event_id_;
	tINT event_flags_[MAX_WORLD_EVENT_FLAGS];
	tINT debuff_quest_count_;
};

#define PASSWD_ERROR_COUNT_MAX		(5)

#pragma pack(pop)
