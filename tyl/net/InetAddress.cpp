/*
 * InetAddress.cpp
 *
 *  Created on: 2017Äê10ÔÂ6ÈÕ
 *      Author: root
 */

#include "InetAddress.h"
#include "../base/Logging.h"

#include <endian.h>
#include <string.h> //bzero()

using namespace tyl;

InetAddress::InetAddress(uint16_t port, bool loopbackOnly)
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    in_addr_t ip = loopbackOnly ? INADDR_LOOPBACK : INADDR_ANY;
    addr_.sin_addr.s_addr = htobe32(ip);
    addr_.sin_port = htobe16(port);
}

InetAddress::InetAddress(std::string ip, uint16_t port)
{
	bzero(&addr_, sizeof addr_);
	addr_.sin_family = AF_INET;
	addr_.sin_port = htobe16(port);
	if (::inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr) <= 0)
	{
		LERROR << "sockets::fromIpPort";
	}
}

InetAddress::InetAddress(uint32_t ip, uint16_t port)
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htobe32(ip);
    addr_.sin_port = htobe16(port);
}

std::string InetAddress::ToIpPort() const
{
  char buf[10] = {0};
  snprintf(buf, sizeof buf, ":%u", GetPort());
  return ToIp().append(buf);
}

std::string InetAddress::ToIp() const
{
	char buf[64] = "";
	::inet_ntop(AF_INET, &addr_.sin_addr, buf, static_cast<socklen_t>(sizeof(buf)));
	return buf;
}
