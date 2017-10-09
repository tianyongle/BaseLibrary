/*
 * Buffer.h
 *
 *  Created on: 2017Äê10ÔÂ5ÈÕ
 *      Author: root
 */

#ifndef TYL_NET_BUFFER_H_
#define TYL_NET_BUFFER_H_

#include <stdint.h>
#include <endian.h>
#include <assert.h>
#include <string.h>
#include <vector>
#include <string>
#include <algorithm>

#include <boost/noncopyable.hpp>

namespace tyl
{
namespace net
{
/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode

class Buffer : boost::noncopyable
{
public:
	explicit Buffer(size_t initialSize = kInitialSize)
	 : buffer_(kCheapPrepend + initialSize), readerIndex_(kCheapPrepend), writerIndex_(kCheapPrepend)
	{
		assert(readableBytes() == 0);
		assert(writableBytes() == initialSize);
		assert(prependableBytes() == kCheapPrepend);
	}

	// implicit copy-ctor, move-ctor, dtor and assignment are fine
	// NOTE: implicit move-ctor is added in g++ 4.6

	void swap(Buffer& rhs)
	{
		buffer_.swap(rhs.buffer_);
		std::swap(readerIndex_, rhs.readerIndex_);
		std::swap(writerIndex_, rhs.writerIndex_);
	}

	size_t readableBytes() const { return writerIndex_ - readerIndex_; }
	size_t writableBytes() const { return buffer_.size() - writerIndex_; }
	size_t prependableBytes() const { return readerIndex_; }

	const char* peek() const { return begin() + readerIndex_; }

	const char* findCRLF() const
	{
		// FIXME: replace with memmem()?
		const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF+2);
		return crlf == beginWrite() ? NULL : crlf;
	}

	const char* findCRLF(const char* start) const
	{
		assert(peek() <= start);
		assert(start <= beginWrite());
		// FIXME: replace with memmem()?
		const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF+2);
		return crlf == beginWrite() ? NULL : crlf;
	}

	const char* findEOL() const
	{
		const void* eol = memchr(peek(), '\n', readableBytes());
		return static_cast<const char*>(eol);
	}

	const char* findEOL(const char* start) const
	{
		assert(peek() <= start);
		assert(start <= beginWrite());
		const void* eol = memchr(start, '\n', beginWrite() - start);
		return static_cast<const char*>(eol);
	}

	// retrieve returns void, to prevent
	// string str(retrieve(readableBytes()), readableBytes());
	// the evaluation of two functions are unspecified
	void retrieve(size_t len)
	{
		assert(len <= readableBytes());
		if(len < readableBytes())
		{
			readerIndex_ += len;
		}
		else
		{
			retrieveAll();
		}
	}

	void retrieveUntil(const char* end)
	{
		assert(peek() <= end);
		assert(end <= beginWrite());
		retrieve(end - peek());
	}

	void retrieveAll()
	{
		readerIndex_ = kCheapPrepend;
		writerIndex_ = kCheapPrepend;
	}

	std::string retrieveAllAsString()
	{
		return retrieveAsString(readableBytes());;
	}

	std::string retrieveAsString(size_t len)
	{
		assert(len <= readableBytes());
		std::string result(peek(), len);
		retrieve(len);
		return result;
	}

	void append(const char* data, size_t len)
	{
		ensureWritableBytes(len);
		std::copy(data, data+len, beginWrite());
		hasWritten(len);
	}

	void append(const void* data, size_t len)
	{
		append(static_cast<const char*>(data), len);
	}

	void ensureWritableBytes(size_t len)
	{
		if (writableBytes() < len)
		{
			makeSpace(len);
		}
		assert(writableBytes() >= len);
	}

	char* beginWrite() { return begin() + writerIndex_; }

	const char* beginWrite() const { return begin() + writerIndex_; }

	void hasWritten(size_t len)
	{
		assert(len <= writableBytes());
		writerIndex_ += len;
	}

	void unwrite(size_t len)
	{
		assert(len <= readableBytes());
		writerIndex_ -= len;
	}

	/// Append int64_t using network endian
	void appendInt64(int64_t x)
	{
		int64_t be64 = htobe64(x);
		append(&be64, sizeof be64);
	}

	/// Append int32_t using network endian
	void appendInt32(int32_t x)
	{
		int32_t be32 = htobe32(x);
		append(&be32, sizeof be32);
	}

	void appendInt16(int16_t x)
	{
		int16_t be16 = htobe16(x);
		append(&be16, sizeof be16);
	}

	void appendInt8(int8_t x)
	{
		append(&x, sizeof x);
	}

	/// Read int64_t from network endian
	int64_t readInt64()
	{
		assert(readableBytes() >= sizeof(int64_t));
		int64_t be64 = 0;
		::memcpy(&be64, peek(), sizeof be64);
		retrieve(sizeof be64);
		return be64toh(be64);
	}

	/// Read int32_t from network endian
	int32_t readInt32()
	{
		assert(readableBytes() >= sizeof(int32_t));
		int32_t be32 = 0;
		::memcpy(&be32, peek(), sizeof be32);
		retrieve(sizeof be32);
		return be32toh(be32);
	}

	/// Read int16_t from network endian
	int16_t readInt16()
	{
		assert(readableBytes() >= sizeof(int16_t));
		int16_t be16 = 0;
		::memcpy(&be16, peek(), sizeof be16);
		retrieve(sizeof be16);
		return be16toh(be16);
	}

	int8_t readInt8()
	{
		assert(readableBytes() >= sizeof(int8_t));
		int8_t be8 = *peek();
		retrieve(sizeof be8);
		return be8;
	}

	/// Prepend int64_t using network endian
	void prependInt64(int64_t x)
	{
		int64_t be64 = htobe64(x);
		prepend(&be64, sizeof be64);
	}

	/// Prepend int32_t using network endian
	void prependInt32(int32_t x)
	{
		int32_t be32 = htobe32(x);
		prepend(&be32, sizeof be32);
	}

	/// Prepend int16_t using network endian
	void prependInt16(int16_t x)
	{
		int16_t be16 = htobe16(x);
		prepend(&be16, sizeof be16);
	}

	void prependInt8(int8_t x)
	{
		prepend(&x, sizeof x);
	}

	void prepend(const void* data, size_t len)
	{
		assert(len <= prependableBytes());
		readerIndex_ -= len;
		const char* d = static_cast<const char*>(data);
		std::copy(d, d+len, begin()+readerIndex_);
	}

	size_t internalCapacity() const
	{
		return buffer_.capacity();
	}

	/// Read data directly into buffer.
	///
	/// It may implement with readv(2)
	/// @return result of read(2), @c errno is saved
	ssize_t readFd(int fd, int* savedErrno);

private:
	char* begin() { return &*buffer_.begin(); }
	const char* begin() const { return &*buffer_.begin(); }

	void makeSpace(size_t len)
	{
		if (writableBytes() + prependableBytes() < len + kCheapPrepend)
		{
			// FIXME: move readable data
			buffer_.resize(writerIndex_+len);
		}
		else
		{
			// move readable data to the front, make space inside buffer
			assert(kCheapPrepend < readerIndex_);
			size_t readable = readableBytes();
			std::copy(begin()+readerIndex_,
			begin()+writerIndex_,
			begin()+kCheapPrepend);
			readerIndex_ = kCheapPrepend;
			writerIndex_ = readerIndex_ + readable;
			assert(readable == readableBytes());
		}
	}

private:
	std::vector<char> buffer_;
	size_t readerIndex_;
	size_t writerIndex_;

	static const char kCRLF[];
	static const size_t kCheapPrepend;
	static const size_t kInitialSize;
};

} /* namespace net */
} /* namespace tyl */

#endif /* TYL_NET_BUFFER_H_ */
