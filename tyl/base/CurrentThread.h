/*
 * CurrentThread.h
 *
 *  Created on: 2017Äê10ÔÂ4ÈÕ
 *      Author: root
 */

#ifndef TYL_BASE_CURRENTTHREAD_H_
#define TYL_BASE_CURRENTTHREAD_H_

namespace tyl
{
	namespace CurrentThread
	{
		extern __thread long int t_cachedTid;
		extern __thread char t_tidString[32];
		extern __thread int t_tidStringLength;
		extern __thread const char* t_threadName;
		void cacheTid();

		inline int tid()
		{
			if (__builtin_expect(t_cachedTid == 0, 0))
			{
				cacheTid();
			}
			return t_cachedTid;
		}
	} /* namespace CurrentThread */
} /* namespace tyl */

#endif /* TYL_BASE_CURRENTTHREAD_H_ */
