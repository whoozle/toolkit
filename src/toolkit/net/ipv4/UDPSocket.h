#ifndef TOOLKIT_NET_IPV4_UDPSOCKET_H
#define TOOLKIT_NET_IPV4_UDPSOCKET_H

#include <toolkit/net/BaseSocket.h>

namespace TOOLKIT_NS { namespace net { namespace ipv4
{
	struct Endpoint;

	class UDPSocket : public BaseSocket
	{
	public:
		using BaseSocket::BaseSocket;

		UDPSocket(): BaseSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)
		{ }

		UDPSocket(UDPSocket && o): BaseSocket(std::move(o))
		{ }
	};

}}}


#endif
