#include "stdafx.h"

#include "cbilling.h"
#include "myhaSlave.h"

#include "localresponse.h"
#include "localannounce.h"
#include "monitorresponse.h"
#include "logger.h"
#include "cserver_session.h"


CBilling::CBilling() :
	service_(NULL)
{
}

CBilling::~CBilling()
{
	service_ = NULL;
}

tBOOL CBilling::init(rnSocketIOService* service)
{
	service_ = service;

	LOG_DEBUG( "CBilling add %d", service_->GetHandle() );

	return cTRUE;
}

void CBilling::clear()
{
	if(service_)
	{
		LOG_DEBUG( "CBilling delete %d", service_->GetHandle() );
	}

	service_ = NULL;

	myhaSlave::connect_billing_handle_ = bnf::instance()->CreateTimer(100, &myhaSlave::getTimerClass());
}

void CBilling::sendConnectProcessInfo( tINT type, tINT id, tINT channel_id, tINT process_id, const tCHAR* ip )
{
	deliver(MonitorResponse::daemonStartAlert( myhaSlave::groupid(), type, id, channel_id, process_id, ip ) );
}

void CBilling::operate(rnSocketIOService* service)
{
	rnPacket::SP packet(service->GetMessage());
	if (packet == NULL)
	{
		CONNECT_LOG_INFO(server_session_info_, service->ip().c_str(), "$$$ disconnect reason[%s] $$$", service->GetErrorMessage().c_str());
		bnf::instance()->RemoveSession(service->GetHandle());
		clear();
		return;
	}

	if (packet->group() == PGROUP_LOCAL_REQUEST)
	{
		switch(packet->type())
		{
		case PTYPE_LOCAL_SERVER_INFO:
			{
				server_session_info_ = *((TServerInfo*)packet->data());
				deliver( CLocalResponse::serverInfo( SERVER_TYPE_CENTER, myhaSlave::groupid(), 0 ) );

				if( Logger::instance()->service() != NULL )
					sendGlogConnectInfo();
			}
			break;
		}
	}
	else if (packet->group() == PGROUP_LOCAL_RESPONSE)
	{
		switch(packet->type())
		{
		case PTYPE_LOCAL_CONNECTION_INFO:
			{
				listen_info_ = *((TListenInfo*)packet->data());
				sendConectionInfo();
			}
			break;
		}
	}
	else if (packet->group() == PGROUP_MAIL_REQUEST)
	{
		myhaSlave::getServerSessionManager().deliver( SERVER_TYPE_DODB, packet );
	}
}

void CBilling::sendConectionInfo()
{
	myhaSlave::getServerSessionManager().deliver( listen_info_.for_type_,
		CLocalAnnounce::connectionInfo( server_session_info_.type_, server_session_info_.id_,
		server_session_info_.channel_, listen_info_.ip_, listen_info_.port_ ) );
}

void CBilling::sendConectionInfo( CServerSession* server_session )
{
	if( server_session->getType() == listen_info_.for_type_ )
	{
		server_session->deliver( CLocalAnnounce::connectionInfo( server_session_info_.type_, server_session_info_.id_,
			server_session->getChannel(), listen_info_.ip_, listen_info_.port_) );
	}
}

void CBilling::sendGlogConnectInfo()
{
	TListenInfo* listen = Logger::instance()->getListenInfo();
	if ( listen != NULL )
		deliver( CLocalAnnounce::connectionInfo( SERVER_TYPE_GLOG, 0, 0, listen->ip_, listen->port_ ) );
}
