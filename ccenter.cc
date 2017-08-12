#include "stdafx.h"
#include "ccenter.h"

#include "localrequest.h"
#include "localresponse.h"
#include "localannounce.h"
#include "monitorresponse.h"


#include "myhaSlave.h"
#include "cserver_session_manager.h"
#include "logger.h"


//////////////////////////////////////////////////////////////////////////
// CCenter
CCenter::CCenter( rnSocketIOService* service ) : service_( service )
{
	server_session_ = NULL;
}

CCenter::~CCenter()
{
}

void CCenter::operate( rnSocketIOService* service )
{
	rnPacket::SP packet( service->GetMessage() );
	if (packet == NULL)
	{
		if( server_session_ != NULL )
		{
			myhaSlave::getServerSessionManager().removeServerSessoin( server_session_ );

			myhaSlave::getMonitor().deliver( MonitorResponse::daemonEndAlert( server_session_->getProcessID(), server_session_->getIp() ) );

			delete server_session_;
		}

		bnf::instance()->RemoveSession( service->GetHandle() );
		delete this;
		return;
	}

	switch( packet->group() )
	{
	case PGROUP_LOCAL_REQUEST:
		{
			switch ( packet->type() )
			{
			case PTYPE_LOCAL_REALTIME_EVENT_STATUS:
				{
					CLocalRequest::TRealtimeEventStatus* data = reinterpret_cast<CLocalRequest::TRealtimeEventStatus*>(packet->data());
					LOG_INFO( "TRealtimeEventStatus::status_[%d]", data->status_);
					myhaSlave::getMonitor().setRealtimeEvent( data->status_, 0, NULL );
				}
				break;
			}
		}
		break;
	case PGROUP_LOCAL_RESPONSE:
		handleLocalResponse( packet );
		break;
	case PGROUP_LOCAL_ANNOUNCE:
		{
			switch( packet->type() )
			{
			case PTYPE_LOCAL_PROCESS_INFO:
				{
					CLocalAnnounce::TDaemonProcessInfo* info = reinterpret_cast<CLocalAnnounce::TDaemonProcessInfo*>(packet->data());

					if( server_session_ != NULL )
					{
						server_session_->setProcessID( info->process_id_ );

						myhaSlave::getMonitor().deliver(MonitorResponse::daemonStartAlert(
							myhaSlave::groupid(), server_session_->getType(), server_session_->getId(), server_session_->getChannel(),
							server_session_->getProcessID(), server_session_->getIp()));
					}
				}
				break;
			}
		}
		break;
	}

	if( server_session_ != NULL )
		server_session_->packetHandler( packet );
}

void CCenter::handleLocalResponse( rnPacket::SP& packet )
{
	switch( packet->type() )
	{
	case PTYPE_LOCAL_SERVER_INFO:
		{
			if( server_session_ != NULL )
			{
				LOG_ERROR( "already set info." );
				return;
			}

			TServerInfo* server_info = (TServerInfo*)packet->data();
			switch( server_info->type_ )
			{
			case SERVER_TYPE_PROXY:
				server_session_ = new CProxySession( this, server_info );
				break;
			case SERVER_TYPE_HCASH:
				server_session_ = new CHcashSession( this, server_info );
				break;
			case SERVER_TYPE_GCONTAINER:
				{
					if( myhaSlave::isInitCompleted() == cFALSE )
					{
						service_->Close( "not init" );
						return;
					}

					server_session_ = new CContainerSession( this, server_info );
				}
				break;
			case SERVER_TYPE_GMOB:
				{
					if( myhaSlave::isInitCompleted() == cFALSE )
					{
						service_->Close( "not init" );
						return;
					}

					server_session_ = new CMobSession( this, server_info );
				}
				break;
			case SERVER_TYPE_DODB:
				server_session_ = new CDodbSession( this, server_info );
				break;
			case SERVER_TYPE_CHAT:
				server_session_ = new CChatSession( this, server_info );
				break;
			default:
				server_session_ = new CServerSession( this, server_info );
				break;
			}

			// add server
			myhaSlave::getServerSessionManager().addServerSessoin( server_session_ );

			// 새로 접속된 서버에게 접속할 서버주소를 보내준다.
			// send connection info
			myhaSlave::getServerSessionManager().sendConectionInfo( server_session_ );
		}
		break;
	case PTYPE_LOCAL_CONNECTION_INFO:
		{
			if( server_session_ == NULL )
			{
				LOG_ERROR( "unknown server session" );
				return;
			}

			TListenInfo* listen_info = (TListenInfo*)packet->data();
			server_session_->addConnectionInfo( listen_info );

			// 새로운 접속 정보를 접속할 서버에게 보내준다.
			myhaSlave::getServerSessionManager().sendConectionInfo( server_session_, listen_info );

			if( server_session_->getType() == SERVER_TYPE_GLOG && listen_info->for_type_ == SERVER_TYPE_CENTER )
			{
				if (Logger::instance()->service())
				{
					LOG_ERROR( "(old:%d,new:%d) glog duplication error", Logger::instance()->getId(), server_session_->getId() );
					break;
				}

				session_handle chandle = bnf::instance()->CreateConnect(listen_info->ip_, listen_info->port_, Logger::instance());
				if (chandle == SessionBase::INVALID_SESSION_HANDLE)
				{
					LOG_ERROR( "logger(%d) ip(%s) port(%d) connect fail", server_session_->getId(), listen_info->ip_, listen_info->port_ );
					break;
				}

				rnSocketIOService* pService = (rnSocketIOService*)bnf::instance()->GetSessionPointer(chandle);
				Logger::instance()->init(pService);
				Logger::instance()->setListenInfo( listen_info );

				myhaSlave::setConnectFlag( CONNECTED_SERVER_GLOG );

				pService->SendBufferSize(20480);

				myhaSlave::getBilling().sendGlogConnectInfo();
			}
		}
		break;
	}
}

void CCenter::deliver( rnPacket::SP& packet )
{
	if( service_ != NULL )
		service_->deliver( packet );
}

void CCenter::deliver( rnPacket* packet )
{
	rnPacket::SP sp_packet( packet );

	deliver( sp_packet );
}
