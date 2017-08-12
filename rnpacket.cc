#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>

#include "type.h"
#include "rnpacket.h"
#include "logsystem.h"


rnPacket::rnPacket(tSIZE size) : rcount_(0)
{
	pHeader_ = reinterpret_cast<Header*>(buffer_);
	pData_ = buffer_ + PACKET_HEADER_SIZE;

	pHeader_->size_ = PACKET_HEADER_SIZE;
	pHeader_->group_ = 0;
	pHeader_->type_ = 0;
}

rnPacket::rnPacket() : rcount_(0)
{
	pHeader_ = reinterpret_cast<Header*>(buffer_);
	pData_ = buffer_ + PACKET_HEADER_SIZE;
}

rnPacket::~rnPacket()
{
}

rnPacket* rnPacket::copy()
{
	rnPacket *clone = new rnPacket(pHeader_->size_);
	memcpy(clone->buffer_, buffer_, pHeader_->size_);
	return clone;
}

void rnPacket::addBYTE(tBYTE v)
{
	*(buffer_ + pHeader_->size_) = v;
	pHeader_->size_ += sizeof(tBYTE);
}

void rnPacket::addBYTE(tBYTE *v, tSIZE count)
{
	tSIZE size = sizeof(tBYTE) * count;
	memcpy(buffer_ + pHeader_->size_, v, size);
	pHeader_->size_ += size;
}

void rnPacket::addBOOL(tBOOL v)
{
	*(buffer_ + pHeader_->size_) = v;
	pHeader_->size_ += sizeof(tBOOL);
}

void rnPacket::addSINT(tSINT v)
{
	*((tSINT*)(buffer_ + pHeader_->size_)) = v;
	pHeader_->size_ += sizeof(tSINT);
}

void rnPacket::addUSINT(tUSINT v)
{
	*((tUSINT*)(buffer_ + pHeader_->size_)) = v;
	pHeader_->size_ += sizeof(tUSINT);
}

void rnPacket::addINT(tINT v)
{
	*((tINT*)(buffer_ + pHeader_->size_)) = v;
	pHeader_->size_ += sizeof(tINT);
}

void rnPacket::addUINT(tUINT v)
{
	*((tUINT*)(buffer_ + pHeader_->size_)) = v;
	pHeader_->size_ += sizeof(tUINT);
}

void rnPacket::addLONG(tLONG v)
{
	*((tLONG*)(buffer_ + pHeader_->size_)) = v;
	pHeader_->size_ += sizeof(tLONG);
}

void rnPacket::addULONG(tULONG v)
{
	*((tULONG*)(buffer_ + pHeader_->size_)) = v;
	pHeader_->size_ += sizeof(tULONG);
}

void rnPacket::addFLOAT(tFLOAT v)
{
	*((tFLOAT*)(buffer_ + pHeader_->size_)) = v;
	pHeader_->size_ += sizeof(tFLOAT);
}

void rnPacket::addVALUE(const tBYTE *v, tSIZE count)
{
	memcpy(buffer_ + pHeader_->size_, v, count);
	pHeader_->size_ += count;
}

tBOOL rnPacket::getBYTE(tBYTE &rt)
{
	if (rcount_ + sizeof(tBYTE) >= MAX_PACKET_MESSAGE)
		return cFALSE;


	rt = *((tBYTE*)(pData_ + rcount_));
	rcount_ += sizeof(tBYTE);

	return cTRUE;
}

tBOOL rnPacket::getBOOL(tBOOL &rt)
{
	if (rcount_ + sizeof(tBOOL) >= MAX_PACKET_MESSAGE)
		return cFALSE;

	rt = *((tBOOL *)(pData_ + rcount_));
	rcount_ += sizeof(tBOOL);

	return cTRUE;
}

tBOOL rnPacket::getSINT(tSINT &rt)
{
	if (rcount_ + sizeof(tSINT) >= MAX_PACKET_MESSAGE)
		return cFALSE;

	rt = *((tSINT*)(pData_ + rcount_));
	rcount_ += sizeof(tSINT);

	return cTRUE;
}

tBOOL rnPacket::getUSINT(tUSINT &rt)
{
	if (rcount_ + sizeof(tUSINT) >= MAX_PACKET_MESSAGE)
		return cFALSE;

	rt = *((tUSINT*)(pData_ + rcount_));
	rcount_ += sizeof(tUSINT);

	return cTRUE;
}

tBOOL rnPacket::getINT(tINT &rt)
{
	if (rcount_ + sizeof(tINT) >= MAX_PACKET_MESSAGE)
		return cFALSE;

	rt = *((tINT*)(pData_ + rcount_));
	rcount_ += sizeof(tINT);

	return cTRUE;
}

tBOOL rnPacket::getUINT(tUINT &rt)
{
	if (rcount_ + sizeof(tUINT) >= MAX_PACKET_MESSAGE)
		return cFALSE;

	rt = *((tUINT*)(pData_ + rcount_));
	rcount_ += sizeof(tUINT);

	return cTRUE;
}

tBOOL rnPacket::getLONG(tLONG &rt)
{
	if (rcount_ + sizeof(tLONG) >= MAX_PACKET_MESSAGE)
		return cFALSE;

	rt = *((tLONG*)(pData_ + rcount_));
	rcount_ += sizeof(tLONG);

	return cTRUE;
}

tBOOL rnPacket::getULONG(tULONG &rt)
{
	if (rcount_ + sizeof(tULONG) >= MAX_PACKET_MESSAGE)
		return cFALSE;

	rt = *((tULONG*)(pData_ + rcount_));
	rcount_ += sizeof(tULONG);

	return cTRUE;
}

tBOOL rnPacket::getFLOAT(tFLOAT &rt)
{
	if (rcount_ + sizeof(tFLOAT) >= MAX_PACKET_MESSAGE)
		return cFALSE;

	rt = *((tFLOAT*)(pData_ + rcount_));
	rcount_ += sizeof(tFLOAT);

	return cTRUE;
}

tBOOL rnPacket::getVALUE(tBYTE *v, tINT count)
{
	if (rcount_ + count >= MAX_PACKET_MESSAGE)
		return cFALSE;

	memcpy(v, pData_ + rcount_, count);
	rcount_ += count;

	return cTRUE;
}

void rnPacket::reset()
{
	pHeader_->size_ = PACKET_HEADER_SIZE;
	pHeader_->group_ = 0;
	pHeader_->type_ = 0;

	rcount_ = 0;
}

void rnPacket::dumpSimple()
{
	std::string packet_group[PGROUP_MAX_ + 1] =
	{
		"PGROUP_NONE_                 ",
		"PGROUP_GUEST_REQUEST         ",
		"PGROUP_MEMBER_REQUEST        ",
		"PGROUP_GUEST_RESPONSE        ",
		"PGROUP_MEMBER_RESPONSE       ",
		"PGROUP_MEMBER_ANNOUNCE       ",
		"PGROUP_MEMBER_UPDATE         ",
		"PGROUP_MEMBER_DEATH_UPDATE   ",
		"PGROUP_MEMBER_DEATH_ANNOUNCE ",
		"PGROUP_CHAT_GUEST_REQUEST    ",
		"PGROUP_CHAT_GUEST_RESPONSE   ",
		"PGROUP_CHAT_MEMBER_REQUEST   ",
		"PGROUP_CHAT_MEMBER_RESPONSE  ",
		"PGROUP_CHAT_MEMBER_ANNOUNCE  ",
		"PGROUP_GAME_REQUEST          ",
		"PGROUP_GAME_RESPONSE         ",
		"PGROUP_GAME_ANNOUNCE         ",
		"PGROUP_LOCAL_REQUEST         ",
		"PGROUP_LOCAL_RESPONSE        ",
		"PGROUP_LOCAL_ANNOUNCE        ",
		"PGROUP_HCASH_REQUEST         ",
		"PGROUP_HCASH_RESPONSE        ",
		"XXX_PGROUP_GUILD_ANNOUNCE    ",
		"PGROUP_GUILD_UPDATE          ",
		"XXX_PGROUP_GLOG_ANNOUNCE     ",
		"PGROUP_BANK_REQUEST          ",
		"PGROUP_BANK_RESPONSE         ",
		"PGROUP_CONTAINER_BROKER      ",
		"PGROUP_PARTY_UPDATE          ",
		"PGROUP_MONITOR_REQUEST       ",
		"PGROUP_MONITOR_RESPONSE      ",
		"PGROUP_MAIL_REQUEST          ",
		"PGROUP_MAIL_RESPONSE         ",
		"PGROUP_MAIL_ANNOUNCE         ",
		"PGROUP_CONSIGN_REQUEST       ",
		"PGROUP_CONSIGN_RESPONSE      ",
		"PGROUP_CONSIGN_ANNOUNCE      ",
		"PGROUP_CHAT_REQUEST          ",
		"PGROUP_CHAT_RESPONSE         ",
		"PGROUP_CHAT_ANNOUNCE         ",
		"PGROUP_MONITOR_ANNOUNCE      ",
		"PGROUP_SOUL_CONSIGN_REQUEST  ",
		"PGROUP_SOUL_CONSIGN_RESPONSE ",
		"PGROUP_SOUL_CONSIGN_ANNOUNCE ",
		"PGROUP_TCOIN_REQUEST         ",
		"PGROUP_TCOIN_RESPONSE        ",
		"PGROUP_BILLING_REQUEST       ",
		"PGROUP_BILLING_RESPONSE      ",
		"PGROUP_BILLING_ANNOUNCE      ",
		"PGROUP_MAX_                  "
	};

	LOG_DEBUG("Packet Info. Group=[ %2d, %s ] Type=[ %3d ] Size=[ %4d ]",
		pHeader_->group_, packet_group[pHeader_->group_].c_str(), pHeader_->type_, pHeader_->size_);
}

void rnPacket::dump()
{
	LOG_DEBUG("packet dump. size: %d, group: %d, type: %d",
		pHeader_->size_, pHeader_->group_, pHeader_->type_);

	int p = 0;
	while (p < pHeader_->size_)
	{
		char hex_log[256];
		int log_len = 0;
		for (int j = 0; j < 16 && p < pHeader_->size_; ++j)
		{
			log_len += snprintf(hex_log + log_len, 256 - log_len, "%.2X ", buffer_[p++]);
		}

		LOG_DEBUG("%s", hex_log);
	}
}
