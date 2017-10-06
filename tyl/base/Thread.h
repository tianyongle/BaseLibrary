/*
 * Thread.h
 *
 *  Created on: 2017Äê10ÔÂ4ÈÕ
 *      Author: root
 */

#ifndef TYL_BASE_THREAD_H_
#define TYL_BASE_THREAD_H_

#include <pthread.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "Sem.h"
#include "Atomic.h"

namespace tyl
{

class Thread : boost::noncopyable
{
public:
	typedef boost::function<void ()> ThreadFunc;

	explicit Thread(const ThreadFunc&, const std::string& name = std::string());
	~Thread();

	void start();
	void runInThread();

private:
	void setDefaultName();

	bool           started_;
	pthread_t      pthreadId_;
	ThreadFunc     func_;
	std::string    name_;
	Sem            latch_;

	static AtomicInt32 sNum;
};

} /* namespace tyl */

#endif /* TYL_BASE_THREAD_H_ */
