#pragma once

#include "rnsocketioservice.h"
#include "rnsocketiohandler.h"
#include "rnpacket.h"

#include "localresponse.h"

#include "bnf.h"

class Logger : public rnSocketIOHandler
{
	virtual void	operate(rnSocketIOService* service);

public:
	Logger() { service_ = NULL; };
	virtual ~Logger() {};

	static Logger*				instance()
	{
		static Logger logger;
		return &logger;
	};

	tBOOL						init(rnSocketIOService *service)
	{
		service_ = service;
		LOG_DEBUG("glog add %d", service_->GetHandle());

		return cTRUE;
	};

	tINT getId() { return server_session_info_.id_; }

	void						clear()
	{
		if (service_)
		{
			LOG_DEBUG("logger is disconnected");
		}
		else
		{
			LOG_ERROR("logger service is null");
		}
		service_ = NULL;
	};

	tBOOL						deliver(const int key, tTIME time, const char * const fmt, ...);
	tBOOL						deliverCussReport(rnPacket::SP packet);
	tBOOL						deliverBilling(const int key, tTIME time, const char * const fmt, ...);

	rnSocketIOService*			service();
	TServerInfo*				getServerSessionInfo() { return &server_session_info_; }

	TListenInfo*				getListenInfo() { return &server_listen_info_; }
	void						setListenInfo(TListenInfo* server_listen_info) { server_listen_info_ = *server_listen_info; }

protected:
	tBOOL						deliver(rnPacket* packet);
	tBOOL						deliver(rnPacket::SP packet);

protected:
	rnSocketIOService*			service_;

	TServerInfo					server_session_info_;

	TListenInfo					server_listen_info_;
};

inline rnSocketIOService* Logger::service()
{
	return service_;
}

inline void Logger::operate(rnSocketIOService* service)
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
		switch (packet->type())
		{
		case PTYPE_LOCAL_SERVER_INFO:
			server_session_info_ = *((TServerInfo*)packet->data());
			deliver(CLocalResponse::serverInfo(server_session_info_.type_, server_session_info_.id_, server_session_info_.channel_));
			break;

		case PTYPE_LOCAL_END:
			clear();
			break;

		default:
			LOG_ERROR("PGROUP_LOCAL_REQUEST:type(%d) skip packet", packet->type());
			break;
		}
	}
	else
	{
		LOG_ERROR("gt:(%d,%d) skip packet", packet->group(), packet->type());
	}
}

inline tBOOL Logger::deliver(rnPacket* packet)
{
	return deliver(rnPacket::SP(packet));
}

inline tBOOL Logger::deliver(rnPacket::SP packet)
{
	if (service_ != NULL)
	{
		service_->deliver(packet);
		return cTRUE;
	}
	else
	{
		return cFALSE;
	}
}

inline tBOOL Logger::deliver(const int key, tTIME time, const char * const fmt, ...)
{
	rnPacket* packet = new rnPacket(MAX_PACKET_MESSAGE);
	packet->setType(PTYPE_LOG_NORMAL);
	char *p = reinterpret_cast<char *>(packet->data());

	int str_len = snprintf(p, MAX_PACKET_MESSAGE - PACKET_HEADER_SIZE, "%d,%d,", key, (tUINT)time);

	va_list ap;
	va_start(ap, fmt);
	str_len += vsnprintf(p + str_len, MAX_PACKET_MESSAGE - PACKET_HEADER_SIZE - str_len, fmt, ap);
	va_end(ap);

	packet->setDataSizeWithoutHeader(str_len + 1);

	if (deliver(packet) == cFALSE)
	{
		LOG_ERROR("glog down----[%s]", p);
		return cFALSE;
	}

	return cTRUE;
}


inline tBOOL Logger::deliverCussReport(rnPacket::SP packet)
{
	packet->setType(PTYPE_LOG_CUSS_REPORT);

	return deliver(packet);
}

inline tBOOL Logger::deliverBilling(const int key, tTIME time, const char * const fmt, ...)
{
	rnPacket* packet = new rnPacket(MAX_PACKET_MESSAGE);
	packet->setType(PTYPE_LOG_BILLING);
	char *p = reinterpret_cast<char *>(packet->data());

	int str_len = snprintf(p, MAX_PACKET_MESSAGE - PACKET_HEADER_SIZE, "%d,%d,", key, (tUINT)time);

	va_list ap;
	va_start(ap, fmt);
	str_len += vsnprintf(p + str_len, MAX_PACKET_MESSAGE - PACKET_HEADER_SIZE - str_len, fmt, ap);
	va_end(ap);

	packet->setDataSizeWithoutHeader(str_len + 1);

	return deliver(packet);
}
