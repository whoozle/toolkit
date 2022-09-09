#ifndef TOOLKIT_NET_IPV4_UDPSERVERSOCKET_H
#define TOOLKIT_NET_IPV4_UDPSERVERSOCKET_H

#include <toolkit/net/ipv4/UDPSocket.h>
#include <toolkit/net/BaseServerSocket.h>

namespace TOOLKIT_NS { namespace net { namespace ipv4
{

	class UDPServerSocket :
		public BaseServerSocket<UDPSocket>
	{
	public:
		UDPServerSocket(): BaseServerSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)
		{ }
	};

}}}


#endif
