/*
 * Atomic.h
 *
 *  Created on: 2017Äê10ÔÂ2ÈÕ
 *      Author: root
 */

#ifndef TYL_BASE_ATOMIC_H_
#define TYL_BASE_ATOMIC_H_

#include <stdint.h>
#include <boost/noncopyable.hpp>

namespace tyl
{
template<typename T>
class AtomicIntegerT : boost::noncopyable
{
public:
	AtomicIntegerT() : value_(0) { }
	T Get() { return __sync_val_compare_and_swap(&value_, 0, 0); }
	T GetAndAdd(T x) { return __sync_fetch_and_add(&value_, x); }
	T AddAndGet(T x) { return __sync_add_and_fetch(&value_, x); }
	T GetAndSub(T x) { return __sync_fetch_and_sub(&value_, x); }
	T SubAndGet(T x) { return __sync_sub_and_fetch(&value_, x); }
	T IncrementAndGet() { return AddAndGet(1); }
	T DecrementAndGet() { return SubAndGet(1); }
	T GetAndSet(T newValue) { return __sync_lock_test_and_set(&value_, newValue); }
private:
	volatile T value_;
};

typedef AtomicIntegerT<int32_t> AtomicInt32;
typedef AtomicIntegerT<int64_t> AtomicInt64;

} /* namespace tyl */

#endif /* TYL_BASE_ATOMIC_H_ */
