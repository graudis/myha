#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <stdio.h>
#include <stdlib.h>

#include "logsystem.h"

DEFINE_SINGLETON(LogSystem);

#define MAX_LOG_VALUE_STRING_LEN	(1024)

//////////////////////////////////////////////////////////////////////////////////////////
// LogSystem
LogSystem::LogSystem()
{
	createTls(&tls_key);
}

LogSystem::~LogSystem()
{
}

void LogSystem::setSubstitutedValue(const char* name, float value)
{
	char value_str[MAX_LOG_VALUE_STRING_LEN];
	snprintf(value_str, MAX_LOG_VALUE_STRING_LEN, "%f", value);
	setSubstitutedValue(name, value_str);
}

void LogSystem::setSubstitutedValue(const char* name, int value)
{
	char value_str[MAX_LOG_VALUE_STRING_LEN];
	snprintf(value_str, MAX_LOG_VALUE_STRING_LEN, "%d", value);
	setSubstitutedValue(name, value_str);
}

void LogSystem::setSubstitutedValue(const char* name, const char* value)
{
	::setenv(name, value, 1);
}

void LogSystem::setSubstitutedValuef(const char* name, const char* fmt, ...)
{
	char value_str[MAX_LOG_VALUE_STRING_LEN];

	va_list args;
	va_start(args, fmt);
	vsnprintf(value_str, MAX_LOG_VALUE_STRING_LEN, fmt, args);
	va_end(args);

	setSubstitutedValue(name, value_str);
}

void LogSystem::configureBasic()
{
	log4cxx::BasicConfigurator::configure();
}

void LogSystem::configureProperty(const char* filename)
{
	log4cxx::PropertyConfigurator::configure(filename);
}

void LogSystem::configureXml(const char* filename)
{
	log4cxx::xml::DOMConfigurator::configure(filename);
}

tls_key_t LogSystem::getTlsKey()
{
	return getInstance().tls_key;
}

logger_t LogSystem::getLogger(const char* name)
{
	return log4cxx::Logger::getLogger(name);
}

logger_t LogSystem::getRootLogger()
{
	return log4cxx::Logger::getRootLogger();
}

CLogBuffer* LogSystem::getLogBuffer()
{
	tls_key_t tls_key = LogSystem::getTlsKey();

	CLogBuffer* message = (CLogBuffer *)getTlsValue(tls_key);
	if (NULL == message)
	{
		message = new CLogBuffer();
		setTlsValue(tls_key, message);
	}

	return message;
}

//////////////////////////////////////////////////////////////////////////////////////////
// CLogMessage
CLogBuffer::CLogBuffer()
{
	m_message[0] = '\0';
	m_length = 0;
}

CLogBuffer::~CLogBuffer()
{
}

void CLogBuffer::addMessagef(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	m_length += vsnprintf(m_message + m_length, MAX_LOG_MESSAGE_LEN - m_length, fmt, args);
	va_end(args);
}

void CLogBuffer::addMessagev(const char* fmt, va_list args)
{
	m_length += vsnprintf(m_message + m_length, MAX_LOG_MESSAGE_LEN - m_length, fmt, args);
}

void CLogBuffer::reset()
{
	m_message[0] = '\0';
	m_length = 0;
}

/*
#include "logsystem.h"

CLogger		myLogger( "spooky000" );

int main( int argc, char* argv[] )
{
	// 설정파일에 ${logfile} 으로 되있는 부분에 적당한 값을 넣어준다.
	LogSystem::setSubstitutedValuef( "logfile", "login_%s.log", argv[1] );

	// xml로 된 설정파일을 읽는다.
	LogSystem::configureXml( "log.xml" );

	// spooky00 로거에 디버그 로그를 출력한다.
	myLogger.debug( "ssss %d", 1 );

	// 루트 로거에 로그를 출력한다.
	LOG_ERROR( "test root log: %d, %d", 10, 100 );
	LOG_WARN( "test root log: %d, %d", 10, 100 );
	LOG_DEBUG( "test root log: %d, %d", 10, 100 );

	return 0;
}
*/
