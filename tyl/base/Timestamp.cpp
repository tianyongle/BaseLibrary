/*
 * Timestamp.cpp
 *
 *  Created on: 2017Äê10ÔÂ2ÈÕ
 *      Author: root
 */

#include <stdio.h>
#include <inttypes.h>
#include <sys/time.h>
#include "Timestamp.h"

namespace tyl
{

time_t Timestamp::lastTime = 0;
std::string Timestamp::lastFmatTime;

std::string Timestamp::toString() const
{
	char buf[32] = {0};
	int64_t seconds = microSeconds_ / kMicroSecondsPerSecond;
	int64_t microseconds = microSeconds_ % kMicroSecondsPerSecond;
	snprintf(buf, sizeof(buf)-1, "%ld.%06ld", seconds, microseconds);
	return buf;
}

std::string Timestamp::toFormattedString(bool showMicroseconds) const
{
	char buf[32] = {0};
	time_t seconds = static_cast<time_t>(microSeconds_ / kMicroSecondsPerSecond);
	int microseconds = static_cast<int>(microSeconds_ % kMicroSecondsPerSecond);
	if(seconds != lastTime)
	{
		lastTime = seconds;
		struct tm tm_time;
		gmtime_r(&seconds, &tm_time);

		if(showMicroseconds)
		{
			snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
					tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
					tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, microseconds);
		}
		else
		{
			snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
					tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
					tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
		}
		lastFmatTime.clear();
		lastFmatTime.append(buf, 17);
	}
	snprintf(buf, sizeof(buf), ".%6d ", microseconds);
	return std::string(lastFmatTime).append(buf, 8);
}

Timestamp Timestamp::now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t seconds = tv.tv_sec;
  return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}

} /* namespace tyl */
