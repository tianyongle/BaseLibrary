/*
 * Sem.h
 *
 *  Created on: 2017Äê10ÔÂ4ÈÕ
 *      Author: root
 */

#ifndef TYL_BASE_SEM_H_
#define TYL_BASE_SEM_H_

#include <assert.h>
#include <semaphore.h>
#include <boost/noncopyable.hpp>

namespace tyl
{

class Sem : boost::noncopyable
{
public:
	Sem() { assert(sem_init(&sem_, 0, 0) == 0); }
	~Sem() { assert(sem_destroy(&sem_) == 0); }

	void wait() { assert(sem_wait(&sem_) == 0); }
	void post() { assert(sem_post(&sem_) == 0); }
	int value() { int val=0; assert(sem_getvalue(&sem_, &val) == 0); return val; }
private:
	sem_t sem_;
};

} /* namespace tyl */


#endif /* TYL_BASE_SEM_H_ */
