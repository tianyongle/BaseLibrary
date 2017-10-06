/*
 * Thread.cpp
 *
 *  Created on: 2017Äê10ÔÂ4ÈÕ
 *      Author: root
 */

#include <stdio.h>
#include "Thread.h"
#include "Logging.h"
#include "CurrentThread.h"

namespace tyl
{

void* startThread(void* obj)
{
	Thread* data = static_cast<Thread*>(obj);
	data->runInThread();
	return NULL;
}

AtomicInt32 Thread::sNum;

void afterFork()
{
  CurrentThread::t_cachedTid = 0;
  CurrentThread::t_threadName = "thread-main ";
  CurrentThread::tid();
}

class ThreadNameInitializer
{
 public:
  ThreadNameInitializer()
  {
    CurrentThread::t_threadName = "thread-main ";
    CurrentThread::tid();
    pthread_atfork(NULL, NULL, &afterFork);
  }
};

ThreadNameInitializer init;

Thread::Thread(const ThreadFunc& func, const std::string& n)
 : started_(false), pthreadId_(0), func_(func), name_(std::string("thread-").append(n)), latch_()
{
	setDefaultName();
}

Thread::~Thread()
{
	if (started_) { pthread_detach(pthreadId_); }
}

void Thread::setDefaultName()
{
	int num = sNum.IncrementAndGet();
	if (name_.size() < 8)
	{
		char buf[8];
		snprintf(buf, sizeof buf, "%d", num);
		name_.append(buf, strlen(buf));
	}
	name_.append(" ");
}

void Thread::runInThread()
{
	CurrentThread::t_threadName = name_.c_str();
	try
	{
		latch_.post();
		func_();
	}
	catch (...)
	{
		char buf[100];
		snprintf(buf, 100, "unknown exception caught in %s\n", CurrentThread::t_threadName);
		LERROR << buf;
	}
}

void Thread::start()
{
	assert(!started_);
	started_ = true;

	if(pthread_create(&pthreadId_, NULL, &startThread, this))
	{
		started_ = false;
		LFATAL << name_ << "pthread_create failed.";
	}
	else
	{
		latch_.wait();
	}
}

} /* namespace tyl */
