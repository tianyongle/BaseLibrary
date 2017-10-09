/*
 * Timer.cpp
 *
 *  Created on: 2017Äê10ÔÂ7ÈÕ
 *      Author: root
 */

#include "Timer.h"

using namespace tyl;

AtomicInt64 Timer::s_numCreated_;

void Timer::restart(Timestamp now)
{
	if (repeat_)
	{
		expiration_ = AddTime(now, interval_);
	}
	else
	{
		expiration_ = Timestamp::invalid();
	}
}
