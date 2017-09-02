#pragma once

#include <log4cxx/log4cxx.h>
#include <log4cxx/logger.h>
#include <stdarg.h>
#include <string>
#include "singleton.h"
#include "threaddata.h"

typedef log4cxx::LoggerPtr logger_t;

class CLogBuffer;

class LogSystem
{
	DECLARE_SINGLETON(LogSystem);
private:
	LogSystem();
	~LogSystem();

public:
	// 설정 파일을 읽기전에 substituted value를 세팅해야 한다.
	static void setSubstitutedValue(const char* name, float value);
	static void setSubstitutedValue(const char* name, int value);
	static void setSubstitutedValue(const char* name, const char* value);
	static void setSubstitutedValuef(const char* name, const char* fmt, ...);

	static void configureBasic();
	static void configureProperty(const char* filename);
	static void configureXml(const char* filename);

	static tls_key_t getTlsKey();

	static logger_t getLogger(const char* name);
	static logger_t getRootLogger();

	static CLogBuffer* getLogBuffer();

private:
	tls_key_t tls_key;
};

#define MAX_LOG_MESSAGE_LEN	(32768)

class CLogBuffer
{
public:
	CLogBuffer();
	~CLogBuffer();

	void addMessagef(const char* fmt, ...);
	void addMessagev(const char* fmt, va_list args);
	void reset();

	const char* getMessage() { return m_message; }
	size_t getLength() { return m_length; }

private:
	char m_message[MAX_LOG_MESSAGE_LEN];
	size_t m_length;
};

#define LOGGER_LOG_FATAL(logger, ...)\
do {\
	CLogBuffer* _buffer = LogSystem::getLogBuffer();\
	_buffer->addMessagef(__VA_ARGS__); \
	LOG4CXX_FATAL(logger, _buffer->getMessage());\
	_buffer->reset();\
} while(0)

#define LOGGER_LOG_ERROR(logger, ...)\
do {\
	CLogBuffer* _buffer = LogSystem::getLogBuffer();\
	_buffer->addMessagef(__VA_ARGS__);\
	LOG4CXX_ERROR(logger, _buffer->getMessage());\
	_buffer->reset();\
} while(0)

#define LOGGER_LOG_WARN(logger, ...)\
do {\
	CLogBuffer* _buffer = LogSystem::getLogBuffer();\
	_buffer->addMessagef(__VA_ARGS__);\
	LOG4CXX_WARN(logger, _buffer->getMessage());\
	_buffer->reset();\
} while(0)

#define LOGGER_LOG_INFO(logger, ...)\
do {\
	CLogBuffer* _buffer = LogSystem::getLogBuffer();\
	_buffer->addMessagef(__VA_ARGS__ );\
	LOG4CXX_INFO(logger, _buffer->getMessage());\
	_buffer->reset();\
} while(0)

#define LOGGER_LOG_DEBUG(logger, ...)\
do {\
	CLogBuffer* _buffer = LogSystem::getLogBuffer();\
	_buffer->addMessagef(__VA_ARGS__ );\
	LOG4CXX_DEBUG(logger, _buffer->getMessage());\
	_buffer->reset();\
} while(0)

#define LOGGER_LOG_TRACE(logger, ...)\
do {\
	CLogBuffer* _buffer = LogSystem::getLogBuffer();\
	_buffer->addMessagef(__VA_ARGS__);\
	LOG4CXX_TRACE(logger, _buffer->getMessage());\
	_buffer->reset();\
} while(0)

#define LOG_FATAL(...)	LOGGER_LOG_FATAL(LogSystem::getRootLogger(), __VA_ARGS__)
#define LOG_ERROR(...)	LOGGER_LOG_ERROR(LogSystem::getRootLogger(), __VA_ARGS__)
#define LOG_WARN(...)	LOGGER_LOG_WARN(LogSystem::getRootLogger(), __VA_ARGS__)
#define LOG_INFO(...)	LOGGER_LOG_INFO(LogSystem::getRootLogger(), __VA_ARGS__)
#define LOG_DEBUG(...)	LOGGER_LOG_DEBUG(LogSystem::getRootLogger(), __VA_ARGS__)
#define LOG_TRACE(...)	LOGGER_LOG_TRACE(LogSystem::getRootLogger(), __VA_ARGS__)
