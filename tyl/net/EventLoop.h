/*
 * EventLoop.h
 *
 *  Created on: 2017Äê10ÔÂ9ÈÕ
 *      Author: root
 */

#ifndef TYL_NET_EVENTLOOP_H_
#define TYL_NET_EVENTLOOP_H_

#include <vector>

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include "../base/Mutex.h"
#include "../base/CurrentThread.h"
#include "../base/Timestamp.h"


namespace tyl
{

class Channel;
class Poller;
class TimerQueue;

/// Reactor, at most one per thread.
///
/// This is an interface class, so don't expose too much details.
class EventLoop : boost::noncopyable
{
public:
	typedef boost::function<void()> Functor;

	EventLoop();
	~EventLoop();  // force out-line dtor, for scoped_ptr members.

	/// Loops forever.
	/// Must be called in the same thread as creation of the object.
	void loop();

	/// Quits loop.
	/// This is not 100% thread safe, if you call through a raw pointer,
	/// better to call through shared_ptr<EventLoop> for 100% safety.
	void quit();

	/// Runs callback immediately in the loop thread.
	/// It wakes up the loop, and run the cb.
	/// If in the same loop thread, cb is run within the function.
	/// Safe to call from other threads.
	void runInLoop(const Functor& cb);
	/// Queues callback in the loop thread.
	/// Runs after finish pooling.
	/// Safe to call from other threads.
	void queueInLoop(const Functor& cb);
};

} /* namespace tyl */


#endif /* TYL_NET_EVENTLOOP_H_ */
