/*
 * test.cpp
 *
 *  Created on: 2017Äê10ÔÂ2ÈÕ
 *      Author: root
 */

#include "../base/Thread.h"
#include "../base/Atomic.h"
#include "../base/Logging.h"
#include "../base/Condition.h"
#include "../base/Timestamp.h"

#include "../net/Buffer.h"
#include "../net/InetAddress.h"

#include <iostream>
using namespace std;


int main()
{
	tyl::InetAddress addr(INADDR_LOOPBACK, 1234);
	LINFO << addr.ToIpPort();
	return 0;
}


