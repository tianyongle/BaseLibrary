/*
 * Condition.cpp
 *
 *  Created on: 2017Äê10ÔÂ2ÈÕ
 *      Author: root
 */

#include <time.h>
#include <errno.h>

#include "Condition.h"

using namespace tyl;

bool Condition::waitForSeconds(int64_t usec)
{
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);

	const int64_t kNanoSecondsPerSecond = 1000000000;
	int64_t nsec = (usec * 1000) + abstime.tv_nsec;
	abstime.tv_sec += nsec / kNanoSecondsPerSecond;
	abstime.tv_nsec = nsec % kNanoSecondsPerSecond;

	MutexLock lock(mutex_);
	return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getMutex(), &abstime);
}
