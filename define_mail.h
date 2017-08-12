#pragma once

#define MAIL_PAGE_LIST_COUNT_MAX	10
#define MAIL_TOTAL_LIST_COUNT_MAX	100
#define	MAIL_TITLE_STRING_MAX		(30+1)
#define MAIL_CONTENT_STRING_MAX		(100+1)
#define MAIL_NOT_ADD_ITEM_FLAG		255

#define MAIL_PICKUP_DELAY_TIME		( 60 * 10 )
#define MAIL_SEND_EXPIRE_TIME		( 60 * 60 * 24 * 30 )
#define MAIL_READ_EXPIRE_TIME		( 60 * 60 * 24 * 7 )

//////////////////////////////////////////////////////////////////////////
// for mail(전서구)
#pragma pack(push, 1)
struct TMailInfo
{
	tSINT			mail_type_;							//	전서구 타입
	mail_regist_t	regist_no_;							//	전서구 등록 번호
	tINT			expire_time_;						//	전서구 만료시간
	tSINT			mail_read_flag_;					//	읽기 유무
	tBYTE			mail_item_flag_;					//	아이템 포함 유무
	tCHAR			send_actor_name_[STATUS_NAME_MAX];	//	보낸 엑터 이름
	tCHAR			title_[MAIL_TITLE_STRING_MAX];		//	전서구 제목
};
#pragma pack(pop)
