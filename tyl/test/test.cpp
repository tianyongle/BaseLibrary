/*
 * test.cpp
 *
 *  Created on: 2017Äê10ÔÂ2ÈÕ
 *      Author: root
 */

#include "../base/Atomic.h"
#include "../base/Thread.h"
#include "../base/Timer.h"
#include "../base/Logging.h"
#include "../base/Condition.h"
#include "../base/Timestamp.h"

#include "../net/Buffer.h"
#include "../net/InetAddress.h"

#include <set>
#include <iostream>
using namespace std;

void setTest()
{
	typedef pair<tyl::Timestamp, int> Entry;
	set<Entry> test;
	tyl::Timestamp tt = tyl::Timestamp::now();
	test.insert(Entry(AddTime(tt, 9.0), 9));
	test.insert(Entry(AddTime(tt, 3.0), 3));
	test.insert(Entry(AddTime(tt, 7.0), 7));
	test.insert(Entry(AddTime(tt, 1.0), 1));
	test.insert(Entry(AddTime(tt, 5.0), 5));
	set<Entry>::iterator iter = test.begin();
	for(; iter != test.end(); iter++)
	{
		cout << iter->second << endl;
	}
}

int main()
{
	tyl::InetAddress addr(INADDR_LOOPBACK, 1234);
	LINFO << addr.ToIpPort();
	return 0;
}


