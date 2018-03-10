#ifndef TOOLKIT_NET_IPV4_ENDPOINT_H
#define TOOLKIT_NET_IPV4_ENDPOINT_H

#include <toolkit/net/ipv4/Address.h>

TOOLKIT_NS_BEGIN

namespace net { namespace bsd
{
	class Socket;
}}

namespace net { namespace ipv4
{

	struct Endpoint
	{
		ipv4::Address	Address;
		int 			Port;

		Endpoint(ipv4::Address address, int port): Address(address), Port(port)
		{ }

		void Connect(bsd::Socket & socket) const;
		void Bind(bsd::Socket & socket) const;
	};

}}
TOOLKIT_NS_END

#endif
