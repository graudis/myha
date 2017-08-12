#pragma once

// reason define
enum GLOG_REASON
{
	GLOG_WRITE_NONE = -1,							//{로그 안만듦}
	// 로그인 아웃 리즌
	GLOG_LOGINOUT_REASON = 0,						//{로그인아웃}

	// 경험치 리즌
	GLOG_EXP_REASON_DAED = 1000,					//{경험치+ 몹이 죽음}
	GLOG_EXP_REASON_ATTACK,							//{경험치+ 몹을 공격}
	GLOG_EXP_REASON_MISSION_MAP,					//{경험치+ 미션맵}
	GLOG_EXP_REASON_JACKPOT,						//{경험치+ 잭팟}
	GLOG_EXP_REASON_EVENT,							//{경험치+ 복권 사용}
	GLOG_EXP_REASON_QUEST,							//{경험치+ 퀘스트 보상}
	GLOG_EXP_REASON_GM,								//{경험치 GM 명령}
	GLOG_EXP_REASON_ETC,							//{경험치+ 기타}
	GLOG_EXP_REASON_RESTORATION_EXP,				//{경험치+ 복구}
	GLOG_EXP_REASON_LEVEL_EVENT,					//{경험치+ 레벨업 이벤트}
	GLOG_EXP_REASON_CASH_SELF_REVIVE,				//{경험치+ 캐쉬 부활}
	GLOG_EXP_REASON_LEVEL_GRADE_REWARD,				//{경험치+ 레벨등급별 보상}
	GLOG_EXP_REASON_EXP_ITEM,						//{경험치+ 아이템}
	GLOG_EXP_REASON_EXP_TODAY_MISSION,				//{경험치+ 오늘의 미션}
	GLOG_EXP_REASON_EXP_TOWER,						//{경험치+ 사신도}
	GLOG_EXP_REASON_GATHER,							//{경험치+ 채집}

	GLOG_EXP_REASON_ACTOR_DAED = 1100,				//{경험치- 액터가 죽음}
	GLOG_EXP_REASON_LEVEL_LIMIT,					//{경험치- 레벨 제한으로 경험치 버려짐}
	GLOG_EXP_REASON_REGRET,							//{경험치- PK로 카오스}

	// 레벨업 리즌
	GLOG_LEVELUP_REASON_EXP = 2000,					//{레벨업 경험치}
	GLOG_LEVELUP_REASON_GM,							//{레벨업 GM명령}

	// 부활 리즌
	GLOG_REVIVEDEAD_REASON = 3000,					//{부활}
	GLOG_REVIVEDEAD_REASON_KILLED_BY_MOB,			//{몹 한테 죽음}
	GLOG_REVIVEDEAD_REASON_KILLED_BY_USER,			//{유저 한테 죽음}


	GLOG_PK_COUNT_REASON_KILLER = 3500,				//{pk를 해서 수라 증가}
	GLOG_PK_COUNT_REASON_KILLED_BY_USER,			//{pk를 당해서 수라 증가}
	GLOG_PK_COUNT_REASON_SET_NORMAL,				//{수라 리셋}

	// 케릭터 생성 삭제 리즌
	GLOG_CREATEDELETE_REASON = 4000,				//{케릭터 생성삭제}
	GLOG_CREATEDELETE_REASON_CREATE = 4003,			//{케릭터 생성}
	GLOG_CREATEDELETE_REASON_ADMIN_REPAIR,			//{케릭터 어드민이 복구}

	// 잭팟 리즌
	GLOG_JACKPOT_REASON = 5000,						//{잭팟}

	// 잭팟 포인트 리즌
	GLOG_JACKPOT_POINT_REASON_USE = 6000,			//{잭팟포인트- 사용}
	GLOG_JACKPOT_POINT_REASON_MOB,					//{잭팟포인트+ 몹}
	GLOG_JACKPOT_POINT_REASON_QUEST,				//{잭팟포인트+ 퀘스트}
	GLOG_JACKPOT_POINT_REASON_GM,					//{잭팟포인트 GM 명령}
	GLOG_JACKPOT_POINT_REASON_LEVEL_EVENT,			//{잭팟포인트+ 레벨업}
	GLOG_JACKPOT_POINT_REASON_TODAY_MISSION,		//{잭팟포인트+ 오늘의 미션}
	GLOG_JACKPOT_POINT_REASON_TUTORIAL,				//{잭팟포인트+ tutorial}

	// 코인 리즌
	// 코인 증가
	GLOG_COIN_REASON_PICKUP = 7000,					//{코인+ 줏음}+
	GLOG_COIN_REASON_SELL_ITEM,						//{코인+ 아이템 판매}+
	GLOG_COIN_REASON_TRADE,							//{코인+ 교환}
	GLOG_COIN_REASON_QUEST,							//{코인+ 퀘스트 보상}+
	GLOG_COIN_REASON_JACKPOT,						//{코인+ 잭팟}+
	GLOG_COIN_REASON_MISSION,						//{코인+ 미션 보상}+
	GLOG_COIN_REASON_EVENT,							//{코인+ 복권 당첨}+
	GLOG_COIN_REASON_KEEP,							//{코인+ 보관}
	GLOG_COIN_REASON_ITEM_BUY_FAIL,					//{코인+ 아이템 구매 실패}
	GLOG_COIN_REASON_GUILD_CREATE_FAIL,				//{코인+ 문파 창설 실패}
	GLOG_COIN_REASON_MAIL_RECEIVE,					//{코인+ 전서구 받음}
	GLOG_COIN_REASON_GM,							//{코인+ GM 명령}+
	GLOG_COIN_GUILD_CHARGE,							//{코인+ 문파 세금 수입}
	GLOG_COIN_REASON_CHEST,							//{코인+ 상자 열기 성공}+
	GLOG_COIN_BLOOD_TO_COIN,						//{코인+ 혈을 coin으로 변환}+
	GLOG_COIN_REASON_KEEP_OUT_COIN,					//{코인+ 창고에 뺌}
	GLOG_COIN_REASON_REWARD_TOURNAMENT,				//{코인+ 토너먼트 보상}+
	GLOG_COIN_REASON_KILL_AUTO_USER,				//{코인+ 오토범을 죽임}
	GLOG_COIN_REASON_KILL_ASURA,					//{코인+ 수라를 죽임}
	GLOG_COIN_REASON_LEVEL_GRADE_REWARD,			//{코인+ 레벨 등급별 보상}+
	GLOG_COIN_REASON_FROM_GM,						//{코인+ GM이 추가해줌}+
	GLOG_COIN_REASON_CONSIGN_CALCULATE_INC,			//{코인+ 위탁 정산}
	GLOG_COIN_REASON_THE_DICE_WIN,					//{코인+ 운명의 주사위 승리}+
	GLOG_COIN_REASON_TOWER_REWARD,					//{코인+ 달의 탑 보상}+
	GLOG_COIN_REASON_REGION_WAR_WITHDRAW,			//{코인+ 지역전쟁 출금}+
	GLOG_COIN_REASON_SELFSHOP_SELL,					//{코인+ 개인상점에서 판매로 증가}
	GLOG_COIN_REASON_TODAY_MISSION,					//{코인+ 오늘의 미션으로 증가}+
	GLOG_COIN_REASON_MIN_PLAYERS_TOURNAMENT,		//{코인+ 참가자 부족으로 무투의 장 종료되서 참가비 되돌려 받음}+
	GLOG_COIN_REASON_THE_DICE_POINT_LOST_INC_COIN,	//{코인+ 운명의 주사위 다이스 포인트를 잃어 보상으로 코인을 받음}+
	GLOG_COIN_REASON_GUILD_KEEP_OUT_COIN,			//{코인+ 길드 창고에서 돈을 출금}+
	GLOG_COIN_REASON_ACTOR_NPC_SELL,				//{코인+ NPC에게 팻 판매}+

	// 코인 감소
	GLOG_COIN_REASON_BUY_ITEM = 7100,				//{코인- 아이템 구매}-
	GLOG_COIN_REASON_ITEM_UPGRADE,					//{코인- 아이템 업그레이드}-
	GLOG_COIN_REASON_REPAIR_ITEM,					//{코인- 아이템 수리}-
	GLOG_COIN_REASON_ADD_QUEST,						//{코인- 퀘스트 받음}-
	GLOG_COIN_REASON_DROP_ITEM,						//{코인- 돈 떨굼}-
	GLOG_COIN_REASON_GUILD_CREATE,					//{코인- 문파 생성}-
	GLOG_COIN_REASON_GIVE,							//{코인- 기부}
	GLOG_COIN_REASON_GIVE_DONATE,					//{코인- 문파 기부}
	GLOG_COIN_REASON_TELEPORT,						//{코인- 텔레포트 대금}-
	GLOG_COIN_REASON_POTAL,							//{코인- 포탈 사용}-
	GLOG_COIN_REASON_REGRET,						//{코인- 참회}-
	GLOG_COIN_REASON_MAIL_SEND_EXPENSE,				//{코인- 전서구 비용}-
	GLOG_COIN_REASON_CONSIGN_REGIST,				//{코인- 아이템 경매 등록}-
	GLOG_COIN_REASON_CONSIGN_BUY_ITEM,				//{코인- 아이템 경매 구매}
	GLOG_COIN_REASON_CONSIGN_CALCULATE,				//{코인- 아이템 경매 정산}
	GLOG_COIN_REASON_SPECIAL_ABILITY_UPGRADE,		//{코인- 전문 능력 향상}-
	GLOG_COIN_REASON_BLOOD_TO_BLOODCORE,			//{코인- 혈을 혈정으로 바꿈}-
	GLOG_COIN_REASON_MOUNT_RICKSHAW,				//{코인- 인력거 탑승}-
	GLOG_COIN_REASON_AFFECT,						//{코인- 어팩트 사용}-
	GLOG_COIN_REASON_WEAR_ACTIVATOR,				//{코인- 엑티베이터에 장착}-
	GLOG_COIN_REASON_RESTORATION_EXP,				//{코인- 경험치 복구}-
	GLOG_COIN_REASON_KEEP_IN_COIN,					//{코인- 창고에 저장}
	GLOG_COIN_REASON_SAVE_POSITION,					//{코인- 위치 저장}-
	GLOG_COIN_REASON_SOUL_CONSIGN_REGIST,			//{코인- 7혼지서 경매 등록}-
	GLOG_COIN_REASON_SOUL_CONSIGN_CALCULATE,		//{코인- 7혼지서 경매 정산}
	GLOG_COIN_REASON_SOUL_CONSIGN_BUY_ITEM,			//{코인- 7혼지서 경매 구매}
	GLOG_COIN_REASON_CUSS_REPORT,					//{코인- 욕설 신고로 감소}-
	GLOG_COIN_REASON_CASH_SELF_REVIVE,				//{코인- 캐쉬 부활}-
	GLOG_COIN_REASON_CASH_ITEM_SELF_REVIVE,			//{코인- 캐쉬 부활 아이템}-
	GLOG_COIN_REASON_REGISTER_TOURNAMENT,			//{코인- 토너먼트 등록}-
	GLOG_COIN_REASON_DEAD_AUTO_USER,				//{코인- 오토범 상태 죽음}
	GLOG_COIN_REASON_DEATH_IN_ASURA,				//{코인- 수라 상태 죽음}
	GLOG_COIN_REASON_DICE_BETTING,					//{코인- 운명의 주사위에 건 금액}-
	GLOG_COIN_REASON_SKILL_STEAL_COIN,				//{코인- 몹 스킬로 돈 뺐겼음}-
	GLOG_COIN_REASON_ITEM_FUSION,					//{코인- 전승 비용}-
	GLOG_COIN_REASON_DIRECT_UPGRADE,				//{코인- 즉시 강화}-
	GLOG_COIN_REASON_ATTATCH_UPGRADE_DEC,			//{코인- 추가 강화로 인하로 감소}-
	GLOG_COIN_REASON_REGION_WAR_EMPLOY,				//{코인- 지역 전쟁 근위병 고용}-
	GLOG_COIN_REASON_SELFSHOP_BUY,					//{코인- 개인상점에서 구매로 감소}
	GLOG_COIN_REASON_MAIL_SEND,						//{코인- 전서구 보냄}
	GLOG_COIN_REASON_DONATE_RICHES_GUILD_ITEM,		//{코인- 길드에 기부}
	GLOG_COIN_REASON_JOIN_TOURNAMENT,				//{코인- 토너먼트 참가}
	GLOG_COIN_REASON_GUILD_KEEP_IN_COIN,			//{코인- 길드창고에 입금}-
	GLOG_COIN_REASON_MANUFACTURE,					//{코인- 채집 재료 가공}-
	GLOG_COIN_REASON_JEWEL_UPGRADE,					//{코인- 보주 업그레이드 비용}-
	GLOG_COIN_REASON_ACTOR_PET_CONSIGN_REGIST,		//{코인- 팻 경매 등록}-
	GLOG_COIN_REASON_ACTOR_PET_CONSIGN_CALCULATE,	//{코인- 팻 경매 정산}-
	GLOG_COIN_REASON_ACTOR_PET_CONSIGN_BUY_ITEM,	//{코인- 팻 경매 구매}
	GLOG_COIN_REASON_ACTOR_PET_CHANGE_NAME,			//{코인- 팻 이름 변경 비용}+

	// 명성 리즌
	// 명성 증가
	GLOG_FAME_REASON_ITEM_BUY_FAIL = 8000,			//{명성+ 아이템 구매 실패}
	GLOG_FAME_REASON_GUILD_CREATE_FAIL,				//{명성+ 문파 창설 실패}
	GLOG_FAME_REASON_REWARD_TOURNAMENT,				//{명성+ 토너먼트 보상}
	GLOG_FAME_REASON_REGION_WAR_KILL,				//{명성+ 지역 전쟁 중 유저 죽임}
	GLOG_FAME_REASON_REGION_WAR_CAPTURE,			//{명성+ 지역 전쟁 중 점령}
	GLOG_FAME_REASON_REGION_WAR_WINNER,				//{명성+ 지역 전쟁 승리}
	GLOG_FAME_REASON_REGION_WAR_LOSER,				//{명성+ 지역 전쟁 패배자}
	GLOG_FAME_REASON_MIN_PLAYERS_TOURNAMENT,		//{명성+ 참가자 부족으로 무투의 장 종료되서 참가비 되돌려 받음}+

	//명성 감소
	GLOG_FAME_REASON_BUY_ITEM = 8100,				//{명성- 아이템 구매}
	GLOG_FAME_REASON_GUILD_CREATE,					//{명성- 문파 생성}
	GLOG_FAME_REASON_GIVE_DONATE,					//{명성- 문파 기부}
	GLOG_FAME_REASON_TELEPORT,						//{명성- 텔레포트 대금}
	GLOG_FAME_REASON_POTAL,							//{명성- 포탈 사용}
	GLOG_FAME_REASON_CAOS,							//{명성- PK로 카오스}
	GLOG_FAME_REASON_REGRET,						//{명성- 참회}
	GLOG_FAME_REASON_JOIN_TOURNAMENT,				//{명성- 토너먼트 참가}

	// 바디 포인트 리즌
	GLOG_BODYPOINT_REASON_PASSIVE_DISTRIBUTE = 9000,//{바디포인트+ 유저 선택 분배}
	GLOG_BODYPOINT_REASON_AUTO_DISTRIBUTE,			//{바디포인트+ 자동 분배}
	GLOG_BODYPOINT_REASON_RESET_ALL,				//{바디포인트+ 전체 리셋}
	GLOG_BODYPOINT_REASON_RESET_PART,				//{바디포인트+ 부분 리셋}
	GLOG_BODYPOINT_REASON_GM,						//{바디포인트 GM명령}

	// 퀘스트 리즌
	GLOG_QUEST_REASON_NORMAL = 10000,				//{퀘스트}
	GLOG_QUEST_REASON_GM,							//{퀘스트 GM}

	// 아이템 생성 리즌
	GLOG_ITEM_CREATE_REASON = 11000,				//{아이템생성}
	// 	HUNT_MOB_,			// *
	// 	EXCHANGE_ITEM_,		// * 아이템 교환
	// 	MAKE_ITEM_,			// * 아이템 제작
	// 	UPGRADE_,			// *
	// 	BUY_,				// *
	// 	QUEST_,				// *
	// 	LOTTERY_TICKET_,	// *
	// 	INIT_,				// *
	// 	JACKPOT_,			// *
	// 	GM_,				// *
	// 	TUTORIAL_,			// *
	// 	CASH_ITEM_,			// *
	// 	DIVIDE_ITEM_,		// * CTRL + 마우스를 이용하여 아이템을 나누는 작업
	// 	GM_KEEP_,			// * GM이 외부 프로그램(웹등)을 통해서 아이템을 줄때
	// 	COMPENSATION_,		// * 주화를 이용한 아이템 교환
	// 	PRODUCE_,			// * 생산(produce)
	// 	SYSTEM_MAIL_,		// * 시스템에서 발송한 전서구 아이템
	// 	CHEST_,				// * 상자열기로 얻을때
	// 	BLOODCORE_,			// * 혈로 혈정 생성
	// 	UPGRADE_FAIL,		// 강화 실패시 생성되는 아이템
	// 	CUBE_DUPLICATE_,	// 큐브 복제
	// 	CUBE_DISASSEMBLE_,	// 큐브 분해
	// 	CUBE_MIXING_,		// 큐브 조합
	// 	REVENGE_REWARD_,	// 리벤지 보상
	// 	MISSION_REWARD_,	// 미션 보상
	// 	MARK_,				// 칭호
	// 	SEAL_,				// 봉인지서 획득
	// 	SEAL_TO_SOUL_,		// 봉인지서로 7혼지서 획득
	// 	SOUL_TO_SEAL_,		// 7혼지서로 봉인지서 획득
	// 	SOUL_MIX_,			// 7혼지서 조합
	// 	SOUL_UPGRADE_,		// 7혼지서 업그레이드
	//
	// 	COPY_CHAR = 63,		// 운영에 따른 캐릭터 복사

	// 아이템 액션 리즌
	//유입
	GLOG_ITEM_ACTION_REASON_BUY = 12000,				//{아이템+ 구입}+
	GLOG_ITEM_ACTION_REASON_PICK_UP,					//{아이템+ 줏음}+
	GLOG_ITEM_ACTION_REASON_QUEST_MOB_KILL,				//{아이템+ 협행몹 죽임}+
	GLOG_ITEM_ACTION_REASON_QUEST_START,				//{아이템+ 협행시작}+
	GLOG_ITEM_ACTION_REASON_QUEST_REWARD,				//{아이템+ 협행보상}+
	GLOG_ITEM_ACTION_REASON_QUEST_FLAG_UP,				//{아이템+ 협행소임 증가}+
	GLOG_ITEM_ACTION_REASON_PRODUCE_REWARD,				//{아이템+ 생산}+
	GLOG_ITEM_ACTION_REASON_EVENT,						//{아이템+ 이벤트}+
	GLOG_ITEM_ACTION_REASON_GM,							//{아이템+ GM 명령}+
	GLOG_ITEM_ACTION_REASON_GUILD_CREATE_FAIL,			//{아이템+ 길드 생성 실패}
	GLOG_ITEM_ACTION_REASON_EXCHANGEITEM_REWARD,		//{아이템+ 교환으로 얻음}+
	GLOG_ITEM_ACTION_REASON_MAKEITEM_SUCCESS_REWARD,	//{아이템+ 제작 성공}+
	GLOG_ITEM_ACTION_REASON_MAKEITEM_FAIL_REWARD,		//{아이템+ 제작 실패}
	GLOG_ITEM_ACTION_REASON_TRADE,						//{아이템+ 거래}
	GLOG_ITEM_ACTION_REASON_JACKPOT,					//{아이템+ 잭팟}+
	GLOG_ITEM_ACTION_REASON_MAIL_PICKUP,				//{아이템+ 전서구}
	GLOG_ITEM_ACTION_REASON_CONSIGN_BUY,				//{아이템+ 경매 구입}
	GLOG_ITEM_ACTION_REASON_CONSIGN_RECALL,				//{아이템+ 경매 회수}
	GLOG_ITEM_ACTION_REASON_BLOODANDCORE,				//{아이템+ 혈정}+
	GLOG_ITEM_ACTION_REASON_MOVE_IN,					//{아이템+ 이동 IN}
	GLOG_ITEM_ACTION_REASON_SPLIT,						//{아이템+ 분리}
	GLOG_ITEM_ACTION_REASON_UPGRADE_FAIL,				//{아이템+ 강화 실패}+
	GLOG_ITEM_ACTION_REASON_CUBE_DUPLICATE,				//{아이템+ 큐브 복제}+
	GLOG_ITEM_ACTION_REASON_CUBE_DISASSEMBLE,			//{아이템+ 큐브 분해}+
	GLOG_ITEM_ACTION_REASON_CUBE_FIXEDMIXING,			//{아이템+ 큐브 조합}+
	GLOG_ITEM_ACTION_REASON_CUBE_RANDOMMIXING,			//{아이템+ 큐브 랜덤 조합}+
	GLOG_ITEM_ACTION_REASON_SOUL_EVENT,					//{아이템+ 이벤트}+
	GLOG_ITEM_ACTION_REASON_FROM_GUILD_KEEP,			//{아이템+ 길드창고에서}
	GLOG_ITEM_ACTION_REASON_CASH_BUY,					//{아이템+ 캐쉬 구입}+
	GLOG_ITEM_ACTION_REASON_ACTOR_CREATE,				//{아이템+ 액터 생성}+
	GLOG_ITEM_ACTION_REASON_SPECIAL_DISASSEMBLE,		//{아이템+ 해체로 증가}+
	GLOG_ITEM_ACTION_REASON_AVATAR_DISASSEMBLE,			//{아이템+ 아바타 분해}+
	GLOG_ITEM_ACTION_REASON_RANDOM_BOX,					//{아이템+ 랜덤박스 사용으로 획득}+
	GLOG_ITEM_ACTION_REASON_IN_INVENTORY,				//{아이템+ 인벤으로 들어옴}
	GLOG_ITEM_ACTION_REASON_IN_KEEP,					//{아이템+ 창고로 들어옴}
	GLOG_ITEM_ACTION_REASON_PET_TO_ITEM,				//{아이템+ 팻에서 아이템으로 변경}
	GLOG_ITEM_ACTION_REASON_CASH_ITEM_EVENT,			//{아이템+ 이벤트로 케쉬아이템 오픈}+
	GLOG_ITEM_ACTION_REASON_SELFSHOP_BUY,				//{아이템+ 개인상점에서 구입으로 증가}
	GLOG_ITEM_ACTION_REASON_CONSIGN_REGIST_DIVID_INC,	//{아이템+ 경매장 분할등록으로 증가}
	GLOG_ITEM_ACTION_REASON_RANK_UP_GW_INC,				//{아이템+ 성장형 무기 랭크업}-
	GLOG_ITEM_ACTION_REASON_EXCHANGE_GW_INC,			//{아이템+ 성장형 무기 교환}-
	GLOG_ITEM_ACTION_REASON_GW_DISASSEMBLE_INC,			//{아이템+ 분해로 성장형 정화석 획득}-
	GLOG_ITEM_ACTION_REASON_GW_MIXING,					//{아이템+ 변환식으로 성장형 해주석 획득}-
	GLOG_ITEM_ACTION_REASON_GW_NPC_MIXING,				//{아이템+ npc 교환으로 성장형 해주석 획득}-
	GLOG_ITEM_ACTION_REASON_LIMIT_EXCHANGE_ITEM,		//{아이템+ 제한교환 시스템으로 아이템 획득}-
	GLOG_ITEM_ACTION_REASON_GATHER,						//{아이템+ 채집으로 아이템 획득}-
	GLOG_ITEM_ACTION_REASON_MANUFACTURE,				//{아이템+ 채집 재료 가공으로 아이템 획득}-
	GLOG_ITEM_ACTION_REASON_TREASURE_BOX,				//{아이템+ 보물상자 사용으로 아이템 획득}-
	GLOG_ITEM_ACTION_REASON_ABILITY_ITEM_JEWEL_ITEM_INC, //{아이템+ 보주 아이템 합성으로 증가}-
	GLOG_ITEM_ACTION_REASON_CREATE_COMPOSITION_ITEM,	//{아이템+ 아이템 합성}

	//증가
	GLOG_ITEM_ACTION_REASON_SWAP_INC = 12200,		//{아이템+ 합침}

	//이탈
	GLOG_ITEM_ACTION_REASON_TIMER_END = 12400,		//{아이템- 타이머종료 삭제}-
	GLOG_ITEM_ACTION_REASON_DELETE,					//{아이템- 사용자 삭제}-
	GLOG_ITEM_ACTION_REASON_CONSIGN_REGIST,			//{아이템- 경매물품 등록}
	GLOG_ITEM_ACTION_REASON_MAIL_SEND,				//{아이템- 우편물품 전송}
	GLOG_ITEM_ACTION_REASON_MOVE_OUT,				//{아이템- 이동 OUT}
	GLOG_ITEM_ACTION_REASON_PORTAL,					//{아이템- 포탈로 삭제}-
	GLOG_ITEM_ACTION_REASON_INFO_GUILD_KEEP,		//{아이템- 길드창고로}
	GLOG_ITEM_ACTION_REASON_OUT_INVENTORY,			//{아이템+ 인벤에서 나감}
	GLOG_ITEM_ACTION_REASON_OUT_KEEP,				//{아이템+ 창고에서 나감}
	GLOG_ITEM_ACTION_REASON_DESTROY_COMPOSITION_ITEM,//{아이템- 아이템 합성}

	//감소
	GLOG_ITEM_ACTION_REASON_USE = 12600,					//{아이템- 사용 감소}-
	GLOG_ITEM_ACTION_REASON_SKILL_DEC,						//{아이템- 스킬 사용 감소}-
	GLOG_ITEM_ACTION_REASON_SPLIT_DEC,						//{아이템- 나눔 감소}
	GLOG_ITEM_ACTION_REASON_SELL_DEC,						//{아이템- 판매로 감소}-
	GLOG_ITEM_ACTION_REASON_REVIVE_DEC,						//{아이템- 부활로 감소}-
	GLOG_ITEM_ACTION_REASON_PRODUCE_DEC,					//{아이템- 생산 시작}-
	GLOG_ITEM_ACTION_REASON_GUILD_CREATE_DEC,				//{아이템- 길드 생성}-
	GLOG_ITEM_ACTION_REASON_UPGRADE_GATHER_DEC,				//{아이템- 업그레이드 재료}-
	GLOG_ITEM_ACTION_REASON_EXCHANGEITEM_DEC,				//{아이템- 교환 재료 감소}-
	GLOG_ITEM_ACTION_REASON_MAKEITEM_DEC,					//{아이템- 제작 재료 감소}-
	GLOG_ITEM_ACTION_REASON_QUEST_END_DEC,					//{아이템- 협행 종료 취소}-
	GLOG_ITEM_ACTION_REASON_TRADE_DEC,						//{아이템- 거래}
	GLOG_ITEM_ACTION_REASON_GUILD_DONATE_DEC,				//{아이템- 길드 기부}
	GLOG_ITEM_ACTION_REASON_RANDOM_OPTION_UPGRADE_DEC,		//{아이템- 렌덤 옵션 업그레이드}-
	GLOG_ITEM_ACTION_REASON_GM_DEC,							//{아이템- GM 명령어로 감소}-
	GLOG_ITEM_ACTION_REASON_BLOODANDCORE_COIN_CHANGE_DEC,	//{아이템- 혈정 코인으로 환전}-
	GLOG_ITEM_ACTION_REASON_USE_DYEING_DEC,					//{아이템- 염색 아이템 사용}-
	GLOG_ITEM_ACTION_REASON_SWAP_DEC,						//{아이템- 합침}
	GLOG_ITEM_ACTION_REASON_DROP_DEC,						//{아이템- 드롭}-
	GLOG_ITEM_ACTION_REASON_CUBE_FAIL_DUPLICATE,			//{아이템- 큐브 복제 실패}-
	GLOG_ITEM_ACTION_REASON_CUBE_FAIL_FIXEDMIXING,			//{아이템- 큐브 조합 실패}-
	GLOG_ITEM_ACTION_REASON_CUBE_SUCCESS_FIXEDMIXING,		//{아이템- 큐브 조합 성공}-
	GLOG_ITEM_ACTION_REASON_CUBE_FAIL_RANDOMMIXING,			//{아이템- 큐브 조합 실패}-
	GLOG_ITEM_ACTION_REASON_CUBE_SUCCESS_RANDOMMIXING,		//{아이템- 큐브 조합 성공}-
	GLOG_ITEM_ACTION_REASON_CUBE_SUCCESS_DISASSEMBLE,		//{아이템- 큐브 분해 성공}-
	GLOG_ITEM_ACTION_REASON_BLOODANDCORE_CHANGE_DEC,		//{아이템- 혈단으로 환전}-
	GLOG_ITEM_ACTION_REASON_INC_INVENTORY,					//{아이템- 가방 라인 증가}-
	GLOG_ITEM_ACTION_REASON_INC_KEEP,						//{아이템- 창고 라인 증가}-
	GLOG_ITEM_ACTION_REASON_INC_GUILDKEEP,					//{아이템- 길드창고 라인 증가}-
	GLOG_ITEM_ACTION_REASON_INSERT_DUPLICATE_DELETE,		//{아이템- 동일 아이템으로 삭제}-
	GLOG_ITEM_ACTION_REASON_CASH_ITEM_SELF_REVIVE,			//{아이템- 캐쉬 부활 사용}-
	GLOG_ITEM_ACTION_REASON_ALLOPTION_CLEAR_DEC,			//{아이템- 모든 옵션 초기화}-
	GLOG_ITEM_ACTION_REASON_REGISTER_TOURNAMENT,			//{아이템- 토너먼트 등록}-
	GLOG_ITEM_ACTION_REASON_CHANGE_GUILD_NAME_DEC,			//{아이템- 길드 이름 변경}-
	GLOG_ITEM_ACTION_REASON_SPECIAL_DISASSEMBLE_DEC,		//{아이템- 아이템 해체로 감소}-
	GLOG_ITEM_ACTION_REASON_RANDOM_EXPAND_DEC,				//{아이템- 옵션 확장으로 감소}-
	GLOG_ITEM_ACTION_REASON_AVATAR_SUCCESS_DISASSEMBLE,		//{아이템- 아바타 분해 성공}-
	GLOG_ITEM_ACTION_REASON_AVATAR_OPTION_UPGRADE_DEC,		//{아이템- 아바타 옵션 업그레이드}-
	GLOG_ITEM_ACTION_REASON_LEVEL_GRADE_REWARD_DEC,			//{아이템- 레벨 등급별 보상으로}-
	GLOG_ITEM_ACTION_REASON_ITEM_DROP_GUARD,				//{아이템- 드랍 방지 수호부 감소}-
	GLOG_ITEM_ACTION_REASON_ITEM_SEAL_BIND,					//{아이템- 종속 아이템 봉인 감소}-
	GLOG_ITEM_ACTION_REASON_ITEM_UNBIND,					//{아이템- 종속 해제로 열쇠 감소}-
	GLOG_ITEM_ACTION_REASON_RANDOM_OPTION_CLEAR_DEC,		//{아이템- 랜덤 옵션 초기화}-
	GLOG_ITEM_ACTION_REASON_LANDMARK_ITEM_USE,				//{아이템- 대지부 사용으로 아이템 감소}-
	GLOG_ITEM_ACTION_REASON_REMOTE_REPAIR_ITEM_USE,			//{아이템- 원격수리로 원격수리 아이템 감소}-
	GLOG_ITEM_ACTION_REASON_REMOTE_SHOP_ITEM_USE,			//{아이템- 원격상점으로 원격상점 아이템 감소}-
	GLOG_ITEM_ACTION_REASON_CHARM_ITEM_USE,					//{아이템- 이동부적 사용으로 아이템 감소}-
	GLOG_ITEM_ACTION_REASON_PORTALALLAREA_ITEM_USE,			//{아이템- 축지부 사용으로 아이템 감소}-
	GLOG_ITEM_ACTION_REASON_RANDOM_BOX_USE,					//{아이템- 랜덤박스 사용으로 아이템 감소}-
	GLOG_ITEM_ACTION_REASON_ITEM_FUSION_DEC,				//{아이템- 전승 재료 감소}-
	GLOG_ITEM_ACTION_REASON_ITEM_FUSION_SOURCE_DEC,			//{아이템- 전승 소스 아이템 감소}-
	GLOG_ITEM_ACTION_REASON_DIRECT_UPGRADE_DEC,				//{아이템- 즉시 강화로 감소}-
	GLOG_ITEM_ACTION_REASON_ATTATCH_UPGRADE_MATERIAL_DEC,	//{아이템- 추가 강화로 재료 감소}-
	GLOG_ITEM_ACTION_REASON_REFUND_CASH_ITEM,				//{아이템- 케쉬로 환불}-
	GLOG_ITEM_ACTION_REASON_SELFSHOP_SELL,					//{아이템- 개인상점에서 판매로 감소}-
	GLOG_ITEM_ACTION_REASON_COMPLETE_TODAY_MISSION,			//{아이템- 오늘의 미션 완료로 감소}-
	GLOG_ITEM_ACTION_REASON_ACTOR_PET_SKILL_LEARN,			//{아이템- 액터 팻 스킬 배움으로 감소}
	GLOG_ITEM_ACTION_REASON_CONSIGN_REGIST_DIVID_DEC,		//{아이템- 경매장 분할등록으로 감소}
	GLOG_ITEM_ACTION_REASON_ITEM_EVENT_FUSION_SOURCE_DEC,	//{아이템- 이벤트 전승 소스 아이템 감소}-
	GLOG_ITEM_ACTION_REASON_MAP_EVENT_WHIT_ITEM_DEC,		//{아이템- 맵 이벤트 아이템 사용으로 감소}-
	GLOG_ITEM_ACTION_REASON_CHANGE_ACTOR_NAME,				//{아이템- 이름 변경 아이템 사용으로 감소}-
	GLOG_ITEM_ACTION_REASON_CUSTOMIZE_CHAR,					//{아이템- 외형 변경 아이템 사용으로 감소}-
	GLOG_ITEM_ACTION_REASON_WEAR_UPGRADE_DEC,				//{아이템- 장착창 강화로 감소}-
	GLOG_ITEM_ACTION_REASON_PET_FOOD,						//{아이템- 펫 먹이로 감소}-
	GLOG_ITEM_ACTION_REASON_SOUL_UPGRADE,					//{아이템- 7혼 카드 강화 보조제 감소}-
	GLOG_ITEM_ACTION_REASON_UPGRADE_SOURCE_DEC,				//{아이템- 업그레이드 장비 감소}-
	GLOG_ITEM_ACTION_REASON_UPGRADE_ASSIST_DEC,				//{아이템- 업그레이드 보조제 감소}-
	GLOG_ITEM_ACTION_REASON_RANK_UP_GW_MATERIAL_DEC,		//{아이템- 성장형 무기 랭크업 재료 감소}-
	GLOG_ITEM_ACTION_REASON_RANK_UP_GW_DEC,					//{아이템- 성장형 무기 랭크업 무기 감소}-
	GLOG_ITEM_ACTION_REASON_RANK_UP_GW_ASSIST_DEC,			//{아이템- 성장형 무기 랭크업 보조제 감소}-
	GLOG_ITEM_ACTION_REASON_EXCHANGE_GW_MATERIAL_DEC,		//{아이템- 성장형 무기 교환 재료 감소}-
	GLOG_ITEM_ACTION_REASON_EXCHANGE_GW_DEC,				//{아이템- 성장형 무기 교환 감소}-
	GLOG_ITEM_ACTION_REASON_GW_DISASSEMBLE_DEC,				//{아이템- 성장형 정화석 분해 감소}-
	GLOG_ITEM_ACTION_REASON_GW_MIXING_MATERIAL_DEC,			//{아이템- 성장형 해주석 변환 감소}-
	GLOG_ITEM_ACTION_REASON_GW_NPC_MIXING_MATERIAL_DEC,		//{아이템- 성장형 해주석 npc 교환 감소}-
	GLOG_ITEM_ACTION_REASON_LIMIT_EXCHANGE_MATERIAL_DEC,	//{아이템- 제한 교환 시스템으로 감소}-
	GLOG_ITEM_ACTION_REASON_SKILL_USE_AUTO_DEC,				//{아이템- 스킬 사용시 자동 감소}-
	GLOG_ITEM_ACTION_REASON_MANUFACTURE_DEC,				//{아이템- 채집 재료 가공으로 감소}-
	GLOG_ITEM_ACTION_REASON_MAKE_ITEM_STONE_DEC,			//{아이템- 아이템 제작 보조제 사용으로 감소}-
	GLOG_ITEM_ACTION_REASON_UPGRADE_JEWEL_RANDOMOPTION_DEC,	//{아이템- 보주 아이템 능력치 업그레이드로 감소}-
	GLOG_ITEM_ACTION_REASON_REPAIR_JEWEL_ITEM_DEC,			//{아이템- 보주 아이템 수리로 감소}-
	GLOG_ITEM_ACTION_REASON_ABILITY_ITEM_JEWEL_ITEM_DEC,	//{아이템- 보주 아이템 합성으로 감소}-
	GLOG_ITEM_ACTION_REASON_JACKPOT_DEC,					//{아이템- 잭팟 사용으로 쿠폰 감소}-
	GLOG_ITEM_ACTION_REASON_TREASURE_BOX_DEC,				//{아이템- 보물상자 사용으로 보물상자 감소}-
	GLOG_ITEM_ACTION_REASON_TREASURE_BOX_KEY_DEC,			//{아이템- 보물상자 사용으로 열쇠 감소}-
	GLOG_ITEM_ACTION_REASON_ACTOR_PET_REGIST,				//{아이템- 아이템을 팻으로 변환하여 감소}-
	GLOG_ITEM_ACTION_REASON_PET_EVOLUTION,					//{아이템- 팻 진화로 재료 감소}-
	GLOG_ITEM_ACTION_REASON_PET_UTIL_WEAR,					//{아이템- 팻 유틸 장착 감소}-
	GLOG_ITEM_ACTION_REASON_RESET_INS_COOLTIME,				//{아이템- 던전 쿨타임 초기화로 감소}-
	GLOG_ITEM_ACTION_REASON_SUMMON_FRIEND,					//{아이템- 친구 소환 감소}-
	GLOG_ITEM_ACTION_REASON_USER_CHAT_MEGAPHONE,			//{아이템- 메가폰 사용}-
	GLOG_ITEM_ACTION_REASON_GUILD_MEMBER_EXTEND,			//{아이템- 길드 멤버 확장}-

	//기타
	GLOG_ITEM_ACTION_REASON_DEAD_DUR_DEC = 12800,			//{아이템 죽음으로 내구도 감소}
	GLOG_ITEM_ACTION_REASON_ITEM_USE_REPAIR,				//{아이템 수리 아이템으로 수리}
	GLOG_ITEM_ACTION_REASON_GM_REPAIR,						//{아이템 GM명령어로 수리}
	GLOG_ITEM_ACTION_REASON_NPC_TO_REPAIR,					//{아이템 수리상인에게 수리}
	GLOG_ITEM_ACTION_REASON_ATTACK_DUR_DEC,					//{아이템 공격으로 내구도 감소}
	GLOG_ITEM_ACTION_REASON_DEFENCE_DUR_DEC,				//{아이템 방어로 내구도 감소}
	GLOG_ITEM_ACTION_REASON_NORMAL_UPGRADE,					//{아이템 아이템 업그레이드}
	GLOG_ITEM_ACTION_REASON_GM_UPGRADE,						//{아이템 GM명령어 업그레이드}
	GLOG_ITEM_ACTION_REASON_NORMAL_RANDOM_OPTION_UPGRADE,	//{아이템 렌덤옵션 업그레이드}
	GLOG_ITEM_ACTION_REASON_RANDOM_OPTION_UPGRADE_FAIL,		//{아이템 렌덤옵션 업그레이드실패}
	GLOG_ITEM_ACTION_REASON_UNWEAR_SWAP_HAND,				//{아이템 장비 교체로 장착 해제}
	GLOG_ITEM_ACTION_REASON_WEAR_SWAP_HAND,					//{아이템 장비 교체로 장착}
	GLOG_ITEM_ACTION_REASON_UNWEAR_TIMER_END,				//{아이템 시간종료로 장착 해제}
	GLOG_ITEM_ACTION_REASON_UNWEAR_QUEST_END,				//{아이템 퀘스트 종료 장착 해제}
	GLOG_ITEM_ACTION_REASON_ACTOR_DROP,						//{아이템 유저 드롭}
	GLOG_ITEM_ACTION_REASON_MOB_DROP,						//{아이템 몹이 드롭}
	GLOG_ITEM_ACTION_REASON_CHEST_DROP,						//{아이템 상자 열어서 드롭}
	GLOG_ITEM_ACTION_REASON_LAND_TIMER,						//{아이템 땅에서 사라짐}
	GLOG_ITEM_ACTION_REASON_WEAR_MARK_UPGRADE,				//{아이템 칭호템 업그레이드 장착}
	GLOG_ITEM_ACTION_REASON_UNWEAR_MARK_UPGRADE,			//{아이템 칭호템 업그레이드 탈착}
	GLOG_ITEM_ACTION_REASON_MAKE_ITEM_UPGRADE,				//{아이템 제작으로 강화 성공}
	GLOG_ITEM_ACTION_REASON_ALLOPTION_CLEAR,				//{아이템 모든 옵션 초기화}
	GLOG_ITEM_ACTION_REASON_DEAD_AUTO_USER,					//{아이템 오토범 죽으며 떨굼}-
	GLOG_ITEM_ACTION_REASON_SYSTEM_MAIL,					//{아이템 시스템 메일로 보냄}+
	GLOG_ITEM_ACTION_REASON_DESTROY_BY_PK_PENALTY,			//{아이템 PK패널티 종속템 증발}-
	GLOG_ITEM_ACTION_REASON_CHEST_DEC,						//{아이템 상자 열기로 증발}-
	GLOG_ITEM_ACTION_REASON_AVATAR_OPTION_UPGRADE,			//{아이템 아바타 옵션 업그레이드}
	GLOG_ITEM_ACTION_REASON_RANDOM_OPTION_CLEAR,			//{아이템 랜덤 옵션만 초기화}
	GLOG_ITEM_ACTION_REASON_ITEM_FUSION_OPTION_UPGRADE,		//{아이템 전승으로 옵션 업그레이드}
	GLOG_ITEM_ACTION_REASON_DIRECT_UPGRADE,					//{아이템 즉시 강화}
	GLOG_ITEM_ACTION_REASON_UPGRADE_FAIL_NOT_CHANGE_UPGRADE,//{아이템 강화시도 실패(보조제사용)}
	GLOG_ITEM_ACTION_REASON_ATTATCH_UPGRADE,				//{아이템 추가 강화}
	GLOG_ITEM_ACTION_REASON_ALL_WEAR_ITEM_REPAIRE_USE_REPAIRE,	//{아이템 장착아이템 수리권 수리}
	GLOG_ITEM_ACTION_REASON_UPGRADE_JEWEL_RANDOMOPTION,		//{아이템 보주 아이템 능력치 업그레이드}
	GLOG_ITEM_ACTION_REASON_JEWEL_REPAIR,					//{아이템 보주 아이템 수리}
	GLOG_ITEM_ACTION_REASON_ITEM_COMOSITION,				//{아이템 합성 성공으로 업그레이드}

	// 길드 금전 관련 로그
	GLOG_GUILD_COIN_REASON_DONATE_DONATE = 13000,			//{길드금전 기부}
	GLOG_GUILD_COIN_REASON_UPGRADE,							//{길드금전 문파 승급}
	GLOG_GUILD_COIN_REASON_DONATE_SECEDE,					//{길드금전 탈퇴}
	GLOG_GUILD_COIN_REASON_DONATE_PURGE,					//{길드금전 추방}
	GLOG_GUILD_COIN_REASON_PROMOTE_MEMBER,					//{길드금전 문파원 승급}
	GLOG_GUILD_COIN_REASON_DECLARE,							//{길드금전 공성 선포}
	GLOG_GUILD_COIN_REASON_DEVELOP,							//{길드금전 성 개발}
	GLOG_GUILD_COIN_REASON_INVEST,							//{길드금전 성 투자}
	GLOG_GUILD_COIN_REASON_EMPLOY_DEFENCE_MOB,				//{길드금전 수성 몹 고용}
	GLOG_GUILD_COIN_REASON_REPAIR_GATE,						//{길드금전 성문 수리}
	GLOG_GUILD_COIN_REASON_BUY_ITEM,						//{길드금전 아이템 구입}
	GLOG_GUILD_COIN_REASON_RESEARCH,						//{길드금전 연구}

	// 길드 연구 관련 로그
	GLOG_GUILD_RESEARCH_START = 13100,						//{길드연구 시작}
	GLOG_GUILD_RESEARCH_CANCEL,								//{길드연구 취소}
	GLOG_GUILD_RESEARCH_COMPLETE,							//{길드연구 완료}
	GLOG_GUILD_RESEARCH_EXPIRE_COMPLETE,					//{길드연구 완료대기 만료}
	GLOG_GUILD_RESEARCH_USE,								//{길드연구 사용}
	GLOG_GUILD_RESEARCH_EXPIRE_USE,							//{길드연구 사용 만료}

	// 7혼 아이템 관련 로그
	// 유입
	GLOG_SEVEN_ACTION_REASON_ADD_NOMAL_SEAL_RECEIVE = 14000,//{7혼+ 일반 봉인지서}
	GLOG_SEVEN_ACTION_REASON_ADD_FORCE_SEAL_RECEIVE,		//{7혼+ 레벨별 봉인지서}
	GLOG_SEVEN_ACTION_REASON_ADD_SEAL_TO_SOUL,				//{7혼+ 봉인지서 바꿈}
	GLOG_SEVEN_ACTION_REASON_ADD_SOUL_TO_SEAL,				//{7혼+ 봉인지서로 바꿈}
	GLOG_SEVEN_ACTION_REASON_ADD_REGIST_SOUL,				//{7혼+ 등록}
	GLOG_SEVEN_ACTION_REASON_ADD_WEAR,						//{7혼+ 장착}
	GLOG_SEVEN_ACTION_REASON_ADD_UNWEAR,					//{7혼+ 탈착}
	GLOG_SEVEN_ACTION_REASON_ADD_MIX,						//{7혼+ 조합}
	GLOG_SEVEN_ACTION_REASON_ADD_GM,						//{7혼+ GM 명령}
	GLOG_SEVEN_ACTION_REASON_CONSIGN_RECALL,				//{7혼+ 경매 회수}
	GLOG_SEVEN_ACTION_REASON_CONSIGN_BUY,					//{7혼+ 경매 구입}
	GLOG_SEVEN_ACTION_REASON_ADD_CASH_TO_SOUL,				//{7혼+ 캐쉬로 바꿈}
	GLOG_SEVEN_ACTION_REASON_ADD_ITEM_TO_SOUL,				//{7혼+ 아이템으로 바꿈}
	GLOG_SEVEN_ACTION_REASON_ADD_DESASSEMBLE_AVATER,		//{7혼+ 아바타 분해로 획득}

	// 증가 = 14200,
	// 이탈
	GLOG_SEVEN_ACTION_REASON_NOMAL_SEAL_RECEIVE_FULL = 14400,//{7혼- 일반봉인서 꽉참}
	GLOG_SEVEN_ACTION_REASON_FORCE_SEAL_RECEIVE_FULL,		//{7혼- 레벨별 봉인서 꽉참}
	GLOG_SEVEN_ACTION_REASON_SOUL_TO_SEAL_FULL,				//{7혼- 봉인서로 바꿀때 꽉참}
	GLOG_SEVEN_ACTION_REASON_DEL_SEAL_TO_SOUL,				//{7혼- 칠혼서로 바꿈}
	GLOG_SEVEN_ACTION_REASON_DEL_SOUL_TO_SEAL,				//{7혼- 봉인 지서로 바꿈}
	GLOG_SEVEN_ACTION_REASON_DEL_REGIST_SOUL,				//{7혼- 등록}
	GLOG_SEVEN_ACTION_REASON_DEL_WEAR,						//{7혼- 장착}
	GLOG_SEVEN_ACTION_REASON_DEL_UNWEAR,					//{7혼- 탈착}
	GLOG_SEVEN_ACTION_REASON_DEL_MIX,						//{7혼- 조합}
	GLOG_SEVEN_ACTION_REASON_DEL_UPGRADE,					//{7혼- 강화}
	GLOG_SEVEN_ACTION_REASON_USE_COUNT_ZERO,				//{7혼- 사용 완료}
	GLOG_SEVEN_ACTION_REASON_DEL_GM,						//{7혼- GM 명령어}
	GLOG_SEVEN_ACTION_REASON_SCRIPT_ERROR,					//{7혼- 스크립트 오류}
	GLOG_SEVEN_ACTION_REASON_CONSIGN_REGIST,				//{7혼- 경매 등록}
	GLOG_SEVEN_ACTION_REASON_OPTION_UPGRADE,				//{7혼- 아이템 옵션 강화에 사용}

	// 감소
	GLOG_SEVEN_ACTION_REASON_USE_SKILL = 14600,				//{7혼- 스킬 사용 감소}
	GLOG_SEVEN_ACTION_REASON_DEL_FULL_INVEN_BUY_CASH_ITEM,	//{7혼- 캐쉬로 구입시 꽉참}
	// 기타
	GLOG_SEVEN_ACTION_REASON_UPGRADE = 14800,		//{7혼 업그레이드}
	GLOG_SEVEN_ACTION_REASON_GM_UPGRADE,			//{7혼 GM 업그레이드}

	// 전서구
	GLOG_MAIL_SEND = 15000,							//{전서구 보냄}
	GLOG_MAIL_RECIEVE,								//{전서구 받음}
	GLOG_MAIL_READ,									//{전서구 읽음}
	GLOG_MAIL_ITEM_PICKUP,							//{전서구 아이템 픽업}
	GLOG_MAIL_DELETE,								//{전서구 삭제}

	// 경매
	GLOG_CONSIGN_REASON_REGIST = 16000,					//{경매 등록}
	GLOG_CONSIGN_REASON_RECALL,							//{경매 회수}
	GLOG_CONSIGN_REASON_BUY,								//{경매 구매}
	GLOG_CONSIGN_REASON_CALCULATE,							//{경매 정산}

	// 7혼 경매
	GLOG_SOUL_CONSIGN_REGIST = 17000,				//{7혼경매 등록}
	GLOG_SOUL_CONSIGN_RECALL,						//{7혼경매 회수}
	GLOG_SOUL_CONSIGN_BUY,							//{7혼경매 구매}
	GLOG_SOUL_CONSIGN_CALCULATE,					//{7혼경매 정산}

	// 큐브
	GLOG_CUBE_DISASSEMBLE = 18000,					//{큐브 분해}
	GLOG_CUBE_DUPLICATE_SUCCESS,					//{큐브 복제 성공}
	GLOG_CUBE_DUPLICATE_FAIL,						//{큐브 복제 실패}
	GLOG_CUBE_MIXING,								//{큐브 조합}
	GLOG_CUBE_RANDOM_MIXING,						//{큐브 렌덤 조합}

	// 캐쉬 감소
	GLOG_CASH_BUY_SHOP_ITEM_DEC = 19000,			//{캐쉬- 상점 구매}
	GLOG_CASH_INC_INVENTORY_DEC,					//{캐쉬- 인벤 증가}
	GLOG_CASH_INC_KEEP_DEC,							//{캐쉬- 창고 증가}
	GLOG_CASH_SELF_REVIVE_DEC,						//{캐쉬- 자기 부활}
	GLOG_CASH_RECEIVE_CASH_SOUL_ITEM_DEC,			//{캐쉬- 7혼 구입}
	GLOG_CASH_THE_DICE_BETTING,						//{캐쉬- 운명의 주사위 베팅 캐쉬}
	GLOG_CASH_BUY_SHOP_ITEM_GIFT_DEC,				//{캐쉬- 상점 구매(선물)}
	GLOG_CASH_INC_PET_INVENTORY_DEC,				//{캐쉬 팻 인벤 증가}

	GLOG_CASH_REFUND_ITEM_INC = 19200,				//{캐쉬- 아이템 환불}

	// 캐쉬 증가
	GLOG_CASH_INC_THE_DICE_WIN = 19300,				////{캐쉬+ 운명의 주사위로 캐쉬 증가}
	GLOG_CASH_INC_THE_DICE_POINT_CHARGE,			////{캐쉬+ 운명의 주사위포인트 충전}

	// 캐쉬 적용 완료
	GLOG_CASH_BUY_SHOP_ITEM_SUCCESS = 19500,		//{캐쉬 상점 구매 성공}
	GLOG_CASH_BUY_SHOP_ITEM_MAIL_SEND_SUCCESS,		//{캐쉬 상점 구매시 오류로 우편발송}
	GLOG_CASH_INC_INVENTORY_SUCCESS,				//{캐쉬 인벤 증가 성공}
	GLOG_CASH_INC_KEEP_SUCCESS,						//{캐쉬 창고 증가 성공}
	GLOG_CASH_SELF_REVIVE_SUCCESS,					//{캐쉬 자기 부활 성공}
	GLOG_CASH_RECEIVE_CASH_SOUL_ITEM_SUCCESS,		//{캐쉬 7혼 구입 성공}
	GLOG_CASH_THE_DICE_BETTING_SUCCESS,				//{캐쉬 운명의 주사위 베팅 캐쉬 성공}
	GLOG_CASH_REFUND_ITEM_INC_SUCCESS,				//{캐쉬 아이템 환불 성공}
	GLOG_CASH_BUY_SHOP_ITEM_GIFT_SUCCESS,			//{캐쉬 상점 구매 성공(선물)}
	GLOG_CASH_INC_PET_INVENTORY_SUCCESS,			//{캐쉬 팻 인벤 증가 성공}

	// 캐시 증가 적용 완료
	GLOG_CASH_THE_DICE_WIN_SUCCESS = 19600,			//{캐쉬 운명의 주사위 증가 성공}
	GLOG_CASH_THE_DICE_POINT_CHARGE_SUCCESS,		////{캐쉬+ 운명의 주사위포인트 충전 성공}

	// pc방 리즌
	GLOG_PCBANG_CHECK_IN = 20000,					//{PC방 채크 인}
	GLOG_PCBANG_CHECK_OUT,							//{PC방 채크 아웃}
	GLOG_PCBANG_CHECK_OUT_TRY_DUPLICATE,			//{PC방 IP 중복으로 있던유저 강종} 이전 유저 채크아웃
	GLOG_PCBANG_CHECK_OUT_TRY_NOT_FOUND_DUPLICATE,	//{PC방 IP 중복으로 없던유저 강종} 이전 유저 채크아웃
	GLOG_PCBANG_INMAP_PCBANG_APPLY,					//{PC방 인맵시 적용}

	// 아바타
	GLOG_AVATAR_DISASSEMBLE = 21000,				//{아바타 분해}
	GLOG_AVATAR_OPTION_UPGRADE,						//{아바타 옵션 부여}

	// 팻 유입
	GLOG_PET_REASON_ITEM_TO_PET = 22000,			//{팻+ 아이템에서 팻으로 바꿈}
	GLOG_PET_REASON_GM_INSERT,						//{팻+ 지엠 명령어}
	GLOG_PET_REASON_CONSIGN_RECALL,					//{팻+ 경매 회수}
	GLOG_PET_REASON_CONSIGN_BUY,					//{팻+ 경매 구입}

	// 팻 이탈
	GLOG_PET_REASON_PET_TO_ITEM = 22300,			//{팻- 팻에서 아이템으로 바꿈}
	GLOG_PET_REASON_DELETE,							//{팻- 사용자 삭제}
	GLOG_PET_REASON_CONSIGN_REGIST,					//{팻- 경매 등록}
	GLOG_PET_REASON_NPC_SELL,						//{팻- NPC에게 판매}
	GLOG_PET_REASON_EXPIRE_TIME,					//{팻- 시간 만료로 삭제}

	// 강화
	GLOG_ITEM_UPGRADE_REASON_DESTORY = 23000,					// {강화- 파괴확률에 파괴}
	GLOG_ITEM_UPGRADE_REASON_DESTORY_BUT_ASSIST_ITEM_SUCCESS,	// {강화- 파괴확률에 보조재로 성공}
	GLOG_ITEM_UPGRADE_REASON_DESTORY_AND_ASSIST_DESTORY,		// {강화- 파괴확률에 보조재이나 파괴}
	GLOG_ITEM_UPGRADE_REASON_DESTORY_AND_ASSIST_NO_CHANGE_UPGRADE,// {강화- 파괴확률에 보조재로 변화없음}
	GLOG_ITEM_UPGRADE_REASON_DEGRADE,							// {강화- 다운그레이드 확률에 걸림}
	GLOG_ITEM_UPGRADE_REASON_SUCCESS,							// {강화- 성공확률에 성공}
	GLOG_ITEM_UPGRADE_REASON_NO_CHANGE_UPGRADE,					// {강화- 성공확률에 안걸려 변화없음}

	// Raid Mob
	GLOG_RAID_MOB_REGEN_TIME = 24000,							// {레이드몹- 레이드몹의 리젠타임 기록}
	GLOG_RAID_MOB_DEAD_TIME,									// {레이드몹- 레이드몹의 사망타임 기록}
	GLOG_RAID_MOB_SPAWN_TIME,									// {레이드몹- 레이드몹의 스폰타임 기록}

	// Today Mission
	GLOG_TODAY_MISSION_COMPLETE = 24500,						// {오늘의목표- 오늘의 목표 완료타임}

	// cash event item
	GLOG_CASH_EVENT_ITEM_BUY_SHOP_ITEM = 25000,					// {이벤트 캐쉬 상점 구매 성공}

	// Mission Tournament
	GLOG_MISSION_TOURNAMENT_REASON_REG_SEED,					// {무투의장+시드 등록}
	GLOG_MISSION_TOURNAMENT_REASON_UNREG_SEED,					// {무투의장+시드 해제}
	GLOG_MISSION_TOURNAMENT_REASON_UPDATE_SEED,					// {무투의장+시드 변경}
	GLOG_MISSION_TOURNAMENT_REASON_DELETE_SEED,					// {무투의장+불참으로 시드 해제}
	GLOG_MISSION_TOURNAMENT_REASON_JOIN,						// {무투의장+참가}
	GLOG_MISSION_TOURNAMENT_REASON_WIN,							// {무투의장+승리}
	GLOG_MISSION_TOURNAMENT_REASON_LOSE,						// {무투의장+패배}
	GLOG_MISSION_TOURNAMENT_TRY_JOIN,							// {무투의장+입장 시도}
	GLOG_MISSION_TOURNAMENT_FULL,								// {무투의장+인원 제한으로 입장 실패}

	GLOG_GROWTH_WEAPON_RANKUP_SUCCESS = 26000,					// {성장형 무기 - 랭크업 성공}
	GLOG_GROWTH_WEAPON_RANKUP_FAIL,								// {성장형 무기 - 랭크업 실패}
	GLOG_GROWTH_WEAPON_RANKUP_SUCCESS_GM,						// {성장형 무기 - GM 명령으로 랭크업 성공}
	GLOG_GROWTH_WEAPON_RANKUP_FAIL_GM,							// {성장형 무기 - GM 명령으로 랭크업 실패}

	GLOG_GROWTH_WEAPON_EXP_RANKUP_SUCCESS = 26500,				// {성장형 무기 경험치 - 랭크업 성공}
	GLOG_GROWTH_WEAPON_EXP_RANKUP_FAIL,							// {성장형 무기 경험치 - 랭크업 실패}
	GLOG_GROWTH_WEAPON_EXP_GM,									// {성장형 무기 경험치 - GM 명령}
	GLOG_GROWTH_WEAPON_EXP_FOOD_ITEM,							// {성장형 무기 경험치 - 아이템 사용}
	GLOG_GROWTH_WEAPON_EXP_CLEAR,								// {성장형 무기 경험치 - 초기화}
	GLOG_GROWTH_WEAPON_EXP_TIME,								// {성장형 무기 경험치 - 시간으로 정화율 감소}

	GLOG_CASH_AFFECT_START = 27000,								// {어펙트 - 시작}
	GLOG_CASH_AFFECT_RESETTING_TIME,							// {어펙트 - 시간 재설정 (캐쉬 어펙트만)}
	GLOG_CASH_AFFECT_END,										// {어펙트 - 종료}
	GLOG_CASH_AFFECT_END_BLESS,									// {어펙트 - 수호의 가호 종료}
	GLOG_CASH_AFFECT_END_TIME_OUT,								// {어펙트 - 시간 종료}
	GLOG_CASH_AFFECT_END_LOGOUT,								// {어펙트 - 로그아웃 상태에서 종료}
	GLOG_CASH_AFFECT_END_MAP_MOVE,								// {어펙트 - 맵이동으로 종료}
	GLOG_CASH_AFFECT_END_NEED_INDEX,							// {어펙트 - 공간 확보로 종료}
	GLOG_CASH_AFFECT_END_STACK_LESS_SCALAR,						// {어펙트 - 스칼라 낮은 어펙트 종료}
	GLOG_CASH_AFFECT_END_IMMUNE,								// {어펙트 - 면역으로 종료}

	GLOG_LUCKY_ROULETTE_REASON_INC_COUNT = 27100,				// {행운의 룰렛 - 충전}
	GLOG_LUCKY_ROULETTE_REASON_GM_RESET,						// {행운의 룰렛 - gm이 리셋}
	GLOG_LUCKY_ROULETTE_REASON_GM_SET_HAVE_COUNT,				// {행운의 룰렛 - gm이 남은 횟수 셋팅}
	GLOG_LUCKY_ROULETTE_REASON_GM_SET_USE_COUNT,				// {행운의 룰렛 - gm이 사용한 횟수 셋팅}
	GLOG_LUCKY_ROULETTE_REASON_USE,								// {행운의 룰렛 - 사용}


	// 세소2용 리즌 (차후 분류 정리)
	GLOG_SESO2 = 30000,

	// 증가
	GLOG_ITEM_ACTION_REASON_REWARD_ALBUM_INC,

	// 감소
	GLOG_ITEM_ACTION_REASON_EXPAND_SOUL_INVEN_TAB_DEC,
	GLOG_ITEM_ACTION_REASON_EXCHANGE_SOUL_ASSIST_DEC,

	// 증가
	GLOG_SEVEN_ACTION_REASON_WEAR_STACK_ITEM_INC,
	GLOG_SEVEN_ACTION_REASON_UNWEAR_POWER_ITEM_INC,
	GLOG_SEVEN_ACTION_REASON_MOVE_INC,
	GLOG_SEVEN_ACTION_REASON_MERGE_INC,
	GLOG_SEVEN_ACTION_REASON_SWAP_INC,
	GLOG_SEVEN_ACTION_REASON_SPLIT_INC,

	// 감소
	GLOG_SEVEN_ACTION_REASON_DESTROY,
	GLOG_SEVEN_ACTION_REASON_UNWEAR_STACK_ITEM_DEC,
	GLOG_SEVEN_ACTION_REASON_WEAR_STACK_ITEM_DEC,
	GLOG_SEVEN_ACTION_REASON_UNWEAR_POWER_ITEM_DEC,
	GLOG_SEVEN_ACTION_REASON_MOVE_DEC,
	GLOG_SEVEN_ACTION_REASON_MERGE_DEC,
	GLOG_SEVEN_ACTION_REASON_POWER_RECOVERY_DEC,
	GLOG_SEVEN_ACTION_REASON_SWAP_DEC,
	GLOG_SEVEN_ACTION_REASON_SPLIT_DEC,
	GLOG_SEVEN_ACTION_REASON_REGISTER_ALBUM_DEC,
	GLOG_SEVEN_ACTION_REASON_TIMER_DEC,
	GLOG_SEVEN_ACTION_REASON_EXCHANGE_SOUL_NEED_DEC,

	// 채집 레벨업 리즌
	GLOG_GATHER_EXP_UP,							//{채집 경험치 증가}
	GLOG_GATHER_LEVEL_UP_REASON_EXP,			//{채집 레벨업 경험치}

	// 팻 경매
	GLOG_ACTOR_PET_CONSIGN_REASON_REGIST = 31000,					//{팻 경매 등록}
	GLOG_ACTOR_PET_CONSIGN_REASON_RECALL,							//{경매 회수}
	GLOG_ACTOR_PET_CONSIGN_REASON_BUY,								//{경매 구매}
	GLOG_ACTOR_PET_CONSIGN_REASON_CALCULATE,						//{경매 정산}

};
