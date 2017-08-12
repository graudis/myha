#pragma once

#include "MemoryPoolBase.h"
#include "rntype.h"


class SeedCipher;


#define PACKET_HEADER_SIZE	(sizeof(rnPacket::Header))
#define APPEND_PACKET_SIZE	(16)
#define MAX_PACKET_MESSAGE	(10240)

class rnPacket : public MemoryPoolBase<rnPacket>
{
public:
	typedef boost::shared_ptr<rnPacket> SP;

public:
	struct Header
	{
		tUINT		size_		: 16;			//data에 쓰여진 총 길이 (Header size 포함)
		tUINT		group_		: 6;			// 패킷 그룹
		tUINT		type_		: 10;			// 그룹별 타입분류
	};

public:
	rnPacket(tSIZE size);
	rnPacket();
	~rnPacket();

	rnPacket*		copy();
	void			copyHeader(rnPacket* base)		{ *pHeader_ = *base->pHeader_; }

	void			reset();

	void			addBYTE(tBYTE v);
	void			addBYTE(tBYTE *v, tSIZE count);

	void			addBOOL(tBOOL v);

	void			addSINT(tSINT v);

	void			addUSINT(tUSINT v);

	void			addINT(tINT v);

	void			addUINT(tUINT v);

	void			addLONG(tLONG v);

	void			addULONG(tULONG v);

	void			addFLOAT(tFLOAT v);

	void			addVALUE(const tBYTE *v, tSIZE count);

	template<typename T>
	tBOOL			addVALUE(T& v);

	template<typename T>
	tBOOL			addVALUE(const T& v);

	tBOOL			getBYTE(tBYTE &rt);

	tBOOL			getBOOL(tBOOL &rt);

	tBOOL			getSINT(tSINT &rt);

	tBOOL			getUSINT(tUSINT &rt);

	tBOOL			getINT(tINT &rt);

	tBOOL			getUINT(tUINT &rt);

	tBOOL			getLONG(tLONG &rt);

	tBOOL			getULONG(tULONG &rt);

	tBOOL			getFLOAT(tFLOAT &rt);

	tBOOL			getVALUE(tBYTE *v, tINT count);

	tINT			group()										{ return (tINT)pHeader_->group_; }
	tINT			type()										{ return (tINT)pHeader_->type_; }
	tINT			size()										{ return (tINT)pHeader_->size_; }
	tSIZE			dataSize()									{ return (tINT)pHeader_->size_ - PACKET_HEADER_SIZE; }

	rnPacket::Header*	header()								{ return pHeader_; }

	tBYTE*			data()										{ return pData_; }

	tBYTE*			getBuffer()									{ return buffer_; }

	tSIZE			dataMaxSize()								{ return MAX_PACKET_MESSAGE; }

	void			setGroup(tINT group)						{ pHeader_->group_ = group; }
	void			setType(tINT type)							{ pHeader_->type_ = type; }
	void			setGroupType(tINT group, tINT type)			{ pHeader_->group_ = group; pHeader_->type_ = type; }

	void			moveReadCount(tSIZE count)					{ rcount_ += count; }
	void			setReadCount(tSIZE count)					{ rcount_ = count; }

	void			setDataSizeWithoutHeader(tSIZE count)		{ pHeader_->size_ = count + PACKET_HEADER_SIZE; }
	void			setSize(tSIZE count)						{ pHeader_->size_ = count; }

	rnPacket*		compress();
	rnPacket*		decompress();

	void			dumpSimple();
	void			dump();

protected:
	Header*			pHeader_;
	tBYTE*			pData_;
	tSIZE			rcount_;

	tBYTE			buffer_[MAX_PACKET_MESSAGE];
};

template<typename T>
inline tBOOL rnPacket::addVALUE(T& v)
{
	if (MAX_PACKET_MESSAGE < pHeader_->size_ + sizeof(T))
		return cFALSE;

	memcpy(buffer_ + pHeader_->size_, &v, sizeof(T));
	pHeader_->size_ += sizeof(T);

	return cTRUE;
}

template<typename T>
inline tBOOL rnPacket::addVALUE(const T& v)
{
	if (MAX_PACKET_MESSAGE < pHeader_->size_ + sizeof(T))
		return cFALSE;

	memcpy(buffer_ + pHeader_->size_, &v, sizeof(T));
	pHeader_->size_ += sizeof(T);

	return cTRUE;
}
