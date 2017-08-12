#pragma once

#define LOGIN_FOR_CENTER			(9999)
#define LOGIN_FOR_CLIENT			(10000)

#define MONITOR_FOR_WEB				(11000)
#define MONITOR_FOR_CENTER			(11001)
#define MONITOR_FOR_LOGIN			(11002)
#define MONITOR_FOR_BANK			(11003)
#define MONITOR_FOR_CONN_MONITOR	(11004)
#define MONITOR_FOR_BILLING			(11005)

#define BANK_FOR_HCASH				(12000)
#define BANK_FOR_LOGIN				(12001)

#define BILLING_FOR_CENTER			(12100)
#define BILLING_FOR_GCONTAINER		(12101)

#define SERVER_CMD_PORT				(13000)

#define GLOG_PORT(x)				(14000 + (x))
#define RESTORATION_PORT(x)			(14200 + (x))
#define RESTORATION_TEMP_PORT(x)	(14300 + (x))
#define HCASH_FOR_CONTAINER(x)		(14400 + (x))

#define CENTER_PORT(x)				(15000 + (x))

#define CHAT_FOR_CLIENT(x)			(16000 + (x))

#define CHAT_FOR_HCASH(x)			(16100 + (x))
#define CHAT_FOR_DODB(x)			(16200 + (x))

#define HCASH_FOR_PROXY(x)			(17000 + (x))

#define DODB_PORT(x)				(18000 + (x))

#define TCOIN_FOR_PROXY(x)			(19000 + (x))

#define PROXY_FOR_CLIENT(x, y)		(20000 + ((x) * 100) + (y))
#define PROXY_FOR_GCONTAINER(x, y)	(25000 + ((x) * 100) + (y))

#define GCONTAINER_FOR_PROXY(x, y)	(30000 + ((x) * 64) + ((y) * 2))
#define GMOB_FOR_GCONTAINER(x, y)	(30000 + ((x) * 64) + ((y) * 2) + 1)

/*

. login
	9999 (for center)
	10000 (for client)
	10000 + gid (for client)

-------------------------------------
. monitor
	11000 (for web)
	11001 (for center)
	11002 (for login)
	11003 (for bank)

. bank
	12000 (for hcash)

. servercmd
	13000 (for monitor)

--------------------------------------
. glog
	14000 + gid (for all server)

. center
	15000 + gid (for all server)

. chat
	16000 + gid (for gcontainer)
	16500 + gid (for proxy)

. hcash
	17000 + gid (for proxy)

. dodb
	18000 + gid

. proxy
	20000 + (gid * 100) + proxy_id (for client)

--------------------------------------
. gcontainer
	30000 + (mid * 40) + (cid * 2) (for proxy)

. gmob
	30000 + (mid * 40) + (cid * 2) + 1 (for container)

. billing
	12100 (for gcontainer)

*/
