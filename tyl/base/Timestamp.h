/*
 * Timestamp.h
 *
 *  Created on: 2017Äê10ÔÂ2ÈÕ
 *      Author: root
 */

#ifndef TYL_BASE_TIMESTAMP_H_
#define TYL_BASE_TIMESTAMP_H_

#include <string>
#include <stdint.h>

namespace tyl
{

class Timestamp
{
public:
	Timestamp():microSeconds_(0) { }
	Timestamp(int64_t us):microSeconds_(us) { }
	int64_t MicroSeconds() const { return microSeconds_; }

	std::string toString() const;
	std::string toFormattedString(bool showMicroseconds = true) const;

	static Timestamp now();
	static Timestamp invalid() { return Timestamp(); }
	static time_t lastTime;
	static std::string lastFmatTime;
	static const int kMicroSecondsPerMsec = 1000;
	static const int kMicroSecondsPerSecond = 1000000;
private:
	int64_t microSeconds_;
};

inline Timestamp operator+(Timestamp lhs, Timestamp rhs)
{
	return Timestamp(lhs.MicroSeconds() + rhs.MicroSeconds());
}

inline Timestamp operator-(Timestamp lhs, Timestamp rhs)
{
	return Timestamp(lhs.MicroSeconds() - rhs.MicroSeconds());
}

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
	return lhs.MicroSeconds() < rhs.MicroSeconds();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
	return lhs.MicroSeconds() == rhs.MicroSeconds();
}

inline bool operator!=(Timestamp lhs, Timestamp rhs)
{
	return lhs.MicroSeconds() != rhs.MicroSeconds();
}

inline Timestamp AddTime(Timestamp timestamp, double seconds)
{
	int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
	return Timestamp(timestamp.MicroSeconds() + delta);
}

} /* namespace tyl */

#endif /* TYL_BASE_TIMESTAMP_H_ */
