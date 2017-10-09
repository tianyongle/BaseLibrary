/*
 * InetAddress.h
 *
 *  Created on: 2017Äê10ÔÂ6ÈÕ
 *      Author: root
 */

#ifndef TYL_NET_INETADDRESS_H_
#define TYL_NET_INETADDRESS_H_

#include <stdio.h>
#include <arpa/inet.h>
#include <string>

namespace tyl
{
namespace net
{
class InetAddress
{
public:
	explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false);
	explicit InetAddress(std::string ip, uint16_t port);// ip should be "1.2.3.4"
	explicit InetAddress(uint32_t ip, uint16_t port);
	explicit InetAddress(struct sockaddr_in& addr) : addr_(addr){ }

	std::string ToIp() const;
	std::string ToIpPort() const;

	uint32_t GetIp() const { return be32toh(addr_.sin_addr.s_addr); }
	uint16_t GetPort() const { return be16toh(addr_.sin_port); }
	struct sockaddr* GetAddr() { return (struct sockaddr*)(&addr_); }

private:
	struct sockaddr_in addr_;
};

} /* namespace net */
} /* namespace tyl */

#endif /* TYL_NET_INETADDRESS_H_ */
