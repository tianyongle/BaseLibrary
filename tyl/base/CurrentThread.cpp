/*
 * CurrentThread.cpp
 *
 *  Created on: 2017Äê10ÔÂ4ÈÕ
 *      Author: root
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

namespace tyl
{
	namespace CurrentThread
	{
		__thread long int t_cachedTid = 0;
		__thread char t_tidString[32];
		__thread int t_tidStringLength = 6;
		__thread const char* t_threadName = "unknown";

		void cacheTid()
		{
			if (t_cachedTid == 0)
			{
				t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
				t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%6ld ", t_cachedTid);
			}
		}
	} /* namespace CurrentThread */
} /* namespace tyl */


