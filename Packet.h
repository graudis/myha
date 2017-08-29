#pragma once

#include "MemoryPoolBase.h"

#define PACKET_HEADER_SIZE	(sizeof(Packet::Header))
#define APPEND_PACKET_SIZE	(16)
#define MAX_PACKET_MESSAGE	(10240)

class Packet : public MemoryPoolBase<Packet>
{
public:
	typedef boost::shared_ptr<Packet> SP;

public:
	struct Header
	{
		uint32_t	size	: 16;	// data에 쓰여진 총 길이 (Header size 포함)
		uint32_t	group	: 6;	// 패킷 그룹
		uint32_t	type	: 10;	// 그룹별 타입분류
	};

public:
	Packet(size_t size);
	Packet();
	~Packet();

	Packet* copy();
	void copyHeader(Packet* base) { *_header = *base->_header; }

	void reset();

	void addBYTE(uint8_t v);
	void addBYTE(uint8_t *v, size_t count);
	void addBOOL(bool v);
	void addSINT(int16_t v);
	void addUSINT(uint16_t v);
	void addINT(int32_t v);
	void addUINT(uint32_t v);
	void addLONG(int32_t v);
	void addULONG(uint32_t v);
	void addFLOAT(float v);
	void addVALUE(const uint8_t *v, size_t count);
	template<typename T> bool addVALUE(T& v);
	template<typename T> bool addVALUE(const T& v);

	bool getBYTE(uint8_t &rt);
	bool getBOOL(bool &rt);
	bool getSINT(int16_t &rt);
	bool getUSINT(uint16_t &rt);
	bool getINT(int32_t &rt);
	bool getUINT(uint32_t &rt);
	bool getLONG(int32_t &rt);
	bool getULONG(uint32_t &rt);
	bool getFLOAT(float &rt);
	bool getVALUE(uint8_t *v, int32_t count);

	int32_t getGroup() { return (int32_t)_header->group; }
	int32_t getType() { return (int32_t)_header->type; }
	int32_t getSize() { return (int32_t)_header->size; }
	size_t dataSize() { return (int32_t)_header->size - PACKET_HEADER_SIZE; }

	Packet::Header* getHeader() { return _header; }

	uint8_t* data() { return _data; }

	uint8_t* getBuffer() { return _buffer; }

	size_t dataMaxSize() { return MAX_PACKET_MESSAGE; }

	void setGroup(int32_t group) { _header->group = group; }
	void setType(int32_t type) { _header->type = type; }
	void setGroupType(int32_t group, int32_t type) { _header->group = group; _header->type = type; }

	void moveReadCount(size_t count) { _rcount += count; }
	void setReadCount(size_t count) { _rcount = count; }

	void setDataSizeWithoutHeader(size_t count) { _header->size = count + PACKET_HEADER_SIZE; }
	void setSize(size_t count) { _header->size = count; }

	void dumpSimple();
	void dump();

protected:
	Header* _header;
	uint8_t* _data;
	size_t _rcount;

	uint8_t _buffer[MAX_PACKET_MESSAGE];
};

template<typename T>
inline bool Packet::addVALUE(T& v)
{
	if (MAX_PACKET_MESSAGE < _header->size + sizeof(T))
		return false;

	memcpy(_buffer + _header->size, &v, sizeof(T));
	_header->size += sizeof(T);

	return true;
}

template<typename T>
inline bool Packet::addVALUE(const T& v)
{
	if (MAX_PACKET_MESSAGE < _header->size + sizeof(T))
		return false;

	memcpy(_buffer + _header->size, &v, sizeof(T));
	_header->size += sizeof(T);

	return true;
}
