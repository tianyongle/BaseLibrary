/*
 * Condition.h
 *
 *  Created on: 2017Äê10ÔÂ2ÈÕ
 *      Author: root
 */

#ifndef TYL_BASE_CONDITION_H_
#define TYL_BASE_CONDITION_H_

#include <stdint.h>

#include "Mutex.h"

namespace tyl
{
class Condition : public boost::noncopyable
{
public:
	explicit Condition(Mutex& mutex):mutex_(mutex) { assert(pthread_cond_init(&pcond_, 0) == 0); }
	~Condition() { assert(pthread_cond_destroy(&pcond_) == 0); }

	void wait() { assert(pthread_cond_wait(&pcond_, mutex_.getMutex()) == 0); }
	void notify() { assert(pthread_cond_signal(&pcond_) == 0); }
	void notifyAll() { assert(pthread_cond_broadcast(&pcond_) == 0); }

	// returns true if time out, false otherwise.
	bool waitForSeconds(int64_t usec);

private:
	Mutex& mutex_;
	pthread_cond_t pcond_;
};

} /* namespace tyl */

#endif /* TYL_BASE_CONDITION_H_ */
