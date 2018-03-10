#ifndef TOOLKIT_NET_IPV4_ENDPOINT_H
#define TOOLKIT_NET_IPV4_ENDPOINT_H

#include <toolkit/net/ipv4/Address.h>

TOOLKIT_SUBNS2_BEGIN(net, ipv4)

struct Endpoint
{
	ipv4::Address	Address;
	int 			Port;

	Endpoint(ipv4::Address address, int port): Address(address), Port(port)
	{ }
};

TOOLKIT_SUBNS2_END

#endif
