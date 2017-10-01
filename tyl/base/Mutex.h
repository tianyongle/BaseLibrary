/*
 * Mutex.h
 *
 *  Created on: 2017Äê10ÔÂ2ÈÕ
 *      Author: root
 */

#ifndef TYL_BASE_MUTEX_H_
#define TYL_BASE_MUTEX_H_

#include <assert.h>
#include <pthread.h>
#include <boost/noncopyable.hpp>

namespace tyl
{
class Mutex : public boost::noncopyable
{
public:
	explicit Mutex() { assert(pthread_mutex_init(&mutex_, 0) == 0); }
	~Mutex() { assert(pthread_mutex_destroy(&mutex_) == 0); }

	//Only for MutexLock call
	void lock() { assert(pthread_mutex_lock(&mutex_) == 0); }

	//Only for MutexLock call
	void unlock() { assert(pthread_mutex_unlock(&mutex_) == 0); }

	//Only for Condition call
	pthread_mutex_t* getMutex() { return &mutex_; }

private:
	pthread_mutex_t mutex_;
};

class MutexLock : public boost::noncopyable
{
public:
	explicit MutexLock(Mutex& mutex):mutex_(mutex) { mutex_.lock();}
	~MutexLock() { mutex_.unlock();}
private:
	Mutex& mutex_;
};
}

#endif /* TYL_BASE_MUTEX_H_ */
