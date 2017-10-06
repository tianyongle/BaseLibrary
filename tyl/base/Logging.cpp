/*
 * Logging.cpp
 *
 *  Created on: 2017Äê10ÔÂ2ÈÕ
 *      Author: root
 */

#include "../config.h"
#include "Logging.h"
#include "Timestamp.h"
#include "CurrentThread.h"

#include <algorithm>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <boost/type_traits/is_arithmetic.hpp>

namespace tyl
{

const char digits[] = "9876543210123456789ABCDEF";
const char* zero = digits + 9;

template<typename T>
size_t convert(char buf[], T value)
{
	T i = value;
	char* p = buf;

	do
	{
		int lsd = static_cast<int>(i % 10);
		i /= 10;
		*p++ = zero[lsd];
	} while (i != 0);

	if (value < 0)
	{
		*p++ = '-';
	}
	*p = '\0';
	std::reverse(buf, p);
	return p - buf;
}

size_t convertHex(char buf[], uintptr_t value)
{
	uintptr_t i = value;
	char* p = buf;

	do
	{
		int lsd = static_cast<int>(i % 16);
		i /= 16;
		*p++ = zero[lsd];
	} while (i != 0);

	*p = '\0';
	std::reverse(buf, p);

	return p - buf;
}

LogStream& LogStream::operator<<(int v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned int v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(long long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(unsigned long long v)
{
	formatInteger(v);
	return *this;
}

LogStream& LogStream::operator<<(const void* p)
{
	uintptr_t v = reinterpret_cast<uintptr_t>(p);
	if (buffer_.avail() >= kMaxNumericSize)
	{
		char* buf = buffer_.current();
		buf[0] = '0';
		buf[1] = 'x';
		size_t len = convertHex(buf+2, v);
		buffer_.add(len+2);
	}
	return *this;
}

LogStream& LogStream::operator<<(double v)
{
	if (buffer_.avail() >= kMaxNumericSize)
	{
		int len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
		buffer_.add(len);
	}
	return *this;
}

template<typename T>
void LogStream::formatInteger(T v)
{
	if (buffer_.avail() >= kMaxNumericSize)
	{
		size_t len = convert(buffer_.current(), v);
		buffer_.add(len);
	}
}

Logger::LogLevel initLogLevel()
{
  if (::getenv("TYL_LOG_TRACE"))
    return Logger::TRACE;
  else if (::getenv("TYL_LOG_DEBUG"))
    return Logger::DEBUG;
  else
    return Logger::INFO;
}

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{ "TRACE ", "DEBUG ", "INFO  ", "WARNG ", "ERROR ", "FATAL ", };

void defaultOutput(const char* msg, int len)
{
	size_t n = fwrite(msg, 1, len, stdout);
	(void)n;
}

void defaultFlush()
{
  fflush(stdout);
}

Logger::LogLevel g_logLevel = initLogLevel();
Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;

class T
{
public:
	T(const char* str, size_t len):str_(str),len_(len){ assert(strlen(str) == len_); }
	const char* str_;
	const size_t len_;
};

LogStream& operator<<(LogStream& s, T v)
{
	s.append(v.str_, v.len_);
	return s;
}

Logger::Logger(SourceFile file, int line, LogLevel level)
 : line_(line), basename_(file), level_(level)
{
#ifdef LOG_MSG_TIME_INFO
	stream_ << Timestamp::now().toFormattedString();
#endif
#ifdef LOG_MSG_LEVEL_INFO
	stream_ << T(LogLevelName[level_], 6);
#endif
#ifdef LOG_MSG_THREAD_NAME_INFO
	stream_ << CurrentThread::t_threadName;
#endif
}

Logger::Logger(SourceFile file, int line, bool toAbort)
 : line_(line), basename_(file), level_(toAbort ? FATAL:ERROR)
{
#ifdef LOG_MSG_TIME_INFO
	stream_ << Timestamp::now().toFormattedString();
#endif
#ifdef LOG_MSG_LEVEL_INFO
	stream_ << T(LogLevelName[level_], 6);
#endif
#ifdef LOG_MSG_THREAD_NAME_INFO
	stream_ << CurrentThread::t_threadName;
#endif
}

Logger::~Logger()
{
#ifdef LOG_MSG_FILE_LINE_INFO
	stream_ << " - " << basename_ << ':' << line_ << '\n';
#else
	stream_ << '\n';
#endif
	const LogBuffer& buf = stream_.buffer();
	g_output(buf.data(), buf.length());
	if(level_ == FATAL)
	{
		g_flush();
		abort();
	}
}

void Logger::setLogLevel(LogLevel level)
{
	g_logLevel = level;
}

void Logger::setOutput(OutputFunc out)
{
	g_output = out;
}

void Logger::setFlush(FlushFunc flush)
{
	g_flush = flush;
}

} /* namespace tyl */
