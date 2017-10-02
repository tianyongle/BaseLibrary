/*
 * BlockQueue.h
 *
 *  Created on: 2017Äê10ÔÂ2ÈÕ
 *      Author: root
 */

#ifndef TYL_BASE_BLOCKQUEUE_H_
#define TYL_BASE_BLOCKQUEUE_H_

#include "Condition.h"
#include <deque>
#include <assert.h>
#include <boost/noncopyable.hpp>

namespace tyl
{

template<typename T>
class BlockQueue : boost::noncopyable
{
public:
	BlockQueue() : mutex_(), notEmpty_(mutex_), queue_() { }

	void put(const T& x)
	{
		MutexLock lock(mutex_);
		queue_.push_back(x);
		notEmpty_.notify(); // wait morphing saves us
	}

	T take()
	{
		MutexLock lock(mutex_);
		// always use a while-loop, due to spurious wakeup
		while (queue_.empty())
		{
			notEmpty_.wait();
		}
		assert(!queue_.empty());
		T front(queue_.front());
		queue_.pop_front();
		return front;
	}

	size_t size() const
	{
		MutexLock lock(mutex_);
		return queue_.size();
	}

private:
	mutable Mutex mutex_;
	Condition      notEmpty_;
	std::deque<T>  queue_;
};

}

#endif /* TYL_BASE_BLOCKQUEUE_H_ */
