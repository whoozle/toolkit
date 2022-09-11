#ifndef TOOLKIT_NET_IPV4_ENDPOINT_H
#define TOOLKIT_NET_IPV4_ENDPOINT_H

#include <toolkit/net/ipv4/Address.h>
#include <netinet/in.h>

namespace TOOLKIT_NS
{

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

		sockaddr_in GetAddress() const;

		void Connect(bsd::Socket & socket) const;
		void Bind(bsd::Socket & socket) const;
	};

}}}


#endif
