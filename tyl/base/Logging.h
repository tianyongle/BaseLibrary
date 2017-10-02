/*
 * Logging.h
 *
 *  Created on: 2017Äê10ÔÂ2ÈÕ
 *      Author: root
 */

#ifndef TYL_BASE_LOGGING_H_
#define TYL_BASE_LOGGING_H_

#include <string>
#include <assert.h>
#include <string.h> // memcpy
#include <boost/noncopyable.hpp>

namespace tyl
{

class LogBuffer : boost::noncopyable
{
public:
	LogBuffer() : cur_(data_) { }
	~LogBuffer() { }

	void append(const char* buf, size_t len)
	{
		// FIXME: append partially
		if (static_cast<size_t>(avail()) > len)
		{
			memcpy(cur_, buf, len);
			cur_ += len;
		}
	}

	const char* data() const { return data_; }
	int length() const { return static_cast<int>(cur_ - data_); }

	// write to data_ directly
	char* current() { return cur_; }
	int avail() const { return static_cast<int>(end() - cur_); }
	void add(size_t len) { cur_ += len; }

	void reset() { cur_ = data_; }
	void bzero() { ::bzero(data_, sizeof data_); }

private:
	const char* end() const { return data_ + sizeof data_; }

	char data_[1024];
	char* cur_;
};

class LogStream : boost::noncopyable
{
public:
	LogStream& operator<<(bool v)
	{
		buffer_.append(v ? "1" : "0", 1);
		return *this;
	}

	LogStream& operator<<(short);
	LogStream& operator<<(unsigned short);
	LogStream& operator<<(int);
	LogStream& operator<<(unsigned int);
	LogStream& operator<<(long);
	LogStream& operator<<(unsigned long);
	LogStream& operator<<(long long);
	LogStream& operator<<(unsigned long long);

	LogStream& operator<<(const void*);

	LogStream& operator<<(float v)
	{
		*this << static_cast<double>(v);
		return *this;
	}
	LogStream& operator<<(double);

	LogStream& operator<<(char v)
	{
		buffer_.append(&v, 1);
		return *this;
	}

	LogStream& operator<<(const char* str)
	{
		if (str)
		{
			buffer_.append(str, strlen(str));
		}
		else
		{
			buffer_.append("(null)", 6);
		}
		return *this;
	}

	LogStream& operator<<(const unsigned char* str)
	{
		return operator<<(reinterpret_cast<const char*>(str));
	}

	LogStream& operator<<(const std::string& v)
	{
		buffer_.append(v.c_str(), v.size());
		return *this;
	}

	LogStream& operator<<(const LogBuffer& v)
	{
		buffer_.append(v.data(), v.length());
		return *this;
	}

	void append(const char* data, int len) { buffer_.append(data, len); }
	const LogBuffer& buffer() const { return buffer_; }
	void resetBuffer() { buffer_.reset(); }

private:
	template<typename T>
	void formatInteger(T);

	LogBuffer buffer_;

	static const int kMaxNumericSize = 32;
};

class Logger
{
public:
	typedef void (*OutputFunc)(const char* msg, int len);
	typedef void (*FlushFunc)();
	enum LogLevel { TRACE, DEBUG, INFO, WARNG, ERROR, FATAL, NUM_LOG_LEVELS, };

	Logger(LogLevel level);
	Logger(bool toAbort);
	~Logger();

	LogStream& stream() { return stream_; }

	static LogLevel logLevel();
	static void setLogLevel(LogLevel level);
	static void setOutput(OutputFunc);
	static void setFlush(FlushFunc);
private:
	LogLevel level_;
	LogStream stream_;
};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel()
{
  return g_logLevel;
}

//
// CAUTION: do not write:
//
// if (good)
//   LOG_INFO << "Good news";
// else
//   LOG_WARN << "Bad news";
//
// this expends to
//
// if (good)
//   if (logging_INFO)
//     logInfoStream << "Good news";
//   else
//     logWarnStream << "Bad news";
//
#define LTRACE if(tyl::Logger::logLevel() <= tyl::Logger::TRACE) tyl::Logger(tyl::Logger::TRACE).stream()
#define LDEBUG if(tyl::Logger::logLevel() <= tyl::Logger::DEBUG) tyl::Logger(tyl::Logger::DEBUG).stream()
#define LINFO if(tyl::Logger::logLevel() <= tyl::Logger::INFO) tyl::Logger(tyl::Logger::INFO).stream()
#define LWARNG tyl::Logger(tyl::Logger::WARNG).stream()
#define LERROR tyl::Logger(tyl::Logger::ERROR).stream()
#define LFATAL tyl::Logger(tyl::Logger::FATAL).stream()

} /* namespace tyl */

#endif /* TYL_BASE_LOGGING_H_ */
