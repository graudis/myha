#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>

#include "type.h"
#include "rnpacket.h"
#include "logsystem.h"


rnPacket::rnPacket(size_t size) : _rcount(0)
{
	_header = reinterpret_cast<Header*>(_buffer);
	_data = _buffer + PACKET_HEADER_SIZE;

	_header->size = PACKET_HEADER_SIZE;
	_header->group = 0;
	_header->type = 0;
}

rnPacket::rnPacket() : _rcount(0)
{
	_header = reinterpret_cast<Header*>(_buffer);
	_data = _buffer + PACKET_HEADER_SIZE;
}

rnPacket::~rnPacket()
{
}

rnPacket* rnPacket::copy()
{
	rnPacket* clone = new rnPacket(_header->size);
	memcpy(clone->_buffer, _buffer, _header->size);
	return clone;
}

void rnPacket::addBYTE(uint8_t v)
{
	*(_buffer + _header->size) = v;
	_header->size += sizeof(uint8_t);
}

void rnPacket::addBYTE(uint8_t *v, size_t count)
{
	size_t size = sizeof(uint8_t) * count;
	memcpy(_buffer + _header->size, v, size);
	_header->size += size;
}

void rnPacket::addBOOL(bool v)
{
	*(_buffer + _header->size) = v;
	_header->size += sizeof(bool);
}

void rnPacket::addSINT(int16_t v)
{
	*((int16_t*)(_buffer + _header->size)) = v;
	_header->size += sizeof(int16_t);
}

void rnPacket::addUSINT(uint16_t v)
{
	*((uint16_t*)(_buffer + _header->size)) = v;
	_header->size += sizeof(uint16_t);
}

void rnPacket::addINT(int32_t v)
{
	*((int32_t*)(_buffer + _header->size)) = v;
	_header->size += sizeof(int32_t);
}

void rnPacket::addUINT(uint32_t v)
{
	*((uint32_t*)(_buffer + _header->size)) = v;
	_header->size += sizeof(uint32_t);
}

void rnPacket::addLONG(int32_t v)
{
	*((int32_t*)(_buffer + _header->size)) = v;
	_header->size += sizeof(int32_t);
}

void rnPacket::addULONG(uint32_t v)
{
	*((uint32_t*)(_buffer + _header->size)) = v;
	_header->size += sizeof(uint32_t);
}

void rnPacket::addFLOAT(float v)
{
	*((float*)(_buffer + _header->size)) = v;
	_header->size += sizeof(float);
}

void rnPacket::addVALUE(const uint8_t *v, size_t count)
{
	memcpy(_buffer + _header->size, v, count);
	_header->size += count;
}

bool rnPacket::getBYTE(uint8_t &rt)
{
	if (_rcount + sizeof(uint8_t) >= MAX_PACKET_MESSAGE)
		return false;


	rt = *((uint8_t*)(_data + _rcount));
	_rcount += sizeof(uint8_t);

	return true;
}

bool rnPacket::getBOOL(bool &rt)
{
	if (_rcount + sizeof(bool) >= MAX_PACKET_MESSAGE)
		return false;

	rt = *((bool *)(_data + _rcount));
	_rcount += sizeof(bool);

	return true;
}

bool rnPacket::getSINT(int16_t &rt)
{
	if (_rcount + sizeof(int16_t) >= MAX_PACKET_MESSAGE)
		return false;

	rt = *((int16_t*)(_data + _rcount));
	_rcount += sizeof(int16_t);

	return true;
}

bool rnPacket::getUSINT(uint16_t &rt)
{
	if (_rcount + sizeof(uint16_t) >= MAX_PACKET_MESSAGE)
		return false;

	rt = *((uint16_t*)(_data + _rcount));
	_rcount += sizeof(uint16_t);

	return true;
}

bool rnPacket::getINT(int32_t &rt)
{
	if (_rcount + sizeof(int32_t) >= MAX_PACKET_MESSAGE)
		return false;

	rt = *((int32_t*)(_data + _rcount));
	_rcount += sizeof(int32_t);

	return true;
}

bool rnPacket::getUINT(uint32_t &rt)
{
	if (_rcount + sizeof(uint32_t) >= MAX_PACKET_MESSAGE)
		return false;

	rt = *((uint32_t*)(_data + _rcount));
	_rcount += sizeof(uint32_t);

	return true;
}

bool rnPacket::getLONG(int32_t &rt)
{
	if (_rcount + sizeof(int32_t) >= MAX_PACKET_MESSAGE)
		return false;

	rt = *((int32_t*)(_data + _rcount));
	_rcount += sizeof(int32_t);

	return true;
}

bool rnPacket::getULONG(uint32_t &rt)
{
	if (_rcount + sizeof(uint32_t) >= MAX_PACKET_MESSAGE)
		return false;

	rt = *((uint32_t*)(_data + _rcount));
	_rcount += sizeof(uint32_t);

	return true;
}

bool rnPacket::getFLOAT(float &rt)
{
	if (_rcount + sizeof(float) >= MAX_PACKET_MESSAGE)
		return false;

	rt = *((float*)(_data + _rcount));
	_rcount += sizeof(float);

	return true;
}

bool rnPacket::getVALUE(uint8_t *v, int32_t count)
{
	if (_rcount + count >= MAX_PACKET_MESSAGE)
		return false;

	memcpy(v, _data + _rcount, count);
	_rcount += count;

	return true;
}

void rnPacket::reset()
{
	_header->size = PACKET_HEADER_SIZE;
	_header->group = 0;
	_header->type = 0;

	_rcount = 0;
}

void rnPacket::dumpSimple()
{
	std::string packet_group[PGROUP_MAX_ + 1] =
	{
		"PGROUP_NONE_           ",
		"PGROUP_LOCAL_REQUEST   ",
		"PGROUP_LOCAL_RESPONSE  ",
		"PGROUP_LOCAL_ANNOUNCE  ",
		"PGROUP_MONITOR_RESPONSE",
		"PGROUP_MONITOR_ANNOUNCE",
		"PGROUP_MAX_            "
	};

	LOG_DEBUG("Packet Info. Group=[ %2d, %s ] Type=[ %3d ] Size=[ %4d ]",
		_header->group, packet_group[_header->group].c_str(), _header->type, _header->size);
}

void rnPacket::dump()
{
	LOG_DEBUG("packet dump. size: %d, group: %d, type: %d",
		_header->size, _header->group, _header->type);

	int p = 0;
	while (p < _header->size)
	{
		char hex_log[256];
		int log_len = 0;
		for (int j = 0; j < 16 && p < _header->size; ++j)
		{
			log_len += snprintf(hex_log + log_len, 256 - log_len, "%.2X ", _buffer[p++]);
		}

		LOG_DEBUG("%s", hex_log);
	}
}
