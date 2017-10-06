/*
 * Timer.h
 *
 *  Created on: 2017Äê10ÔÂ6ÈÕ
 *      Author: root
 */

#ifndef TYL_BASE_TIMER_H_
#define TYL_BASE_TIMER_H_

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include "Atomic.h"
#include "Timestamp.h"

namespace tyl
{

class Timer : boost::noncopyable
{
public:
	typedef boost::function<void()> TimerCallback;
	Timer(const TimerCallback& cb, Timestamp when, double interval)
	  : callback_(cb),
		expiration_(when),
		interval_(interval),
		repeat_(interval > 0.0),
		sequence_(s_numCreated_.IncrementAndGet())
	{ }
private:
	const TimerCallback callback_;
	Timestamp expiration_;
	const double interval_;
	const bool repeat_;
	const int64_t sequence_;

	static AtomicInt64 s_numCreated_;
};

} /* namespace tyl */

#endif /* TYL_BASE_TIMER_H_ */
