#ifndef TOOLKIT_NET_IPV4_TCPSOCKET_H
#define TOOLKIT_NET_IPV4_TCPSOCKET_H

#include <toolkit/net/BaseSocket.h>

namespace TOOLKIT_NS { namespace net { namespace ipv4
{
	struct Endpoint;

	class TCPSocket : public BaseSocket
	{
	public:
		TCPSocket(int flags = ISocket::DefaultFlags): BaseSocket(AF_INET, SOCK_STREAM, 0, flags)
		{ }
	};

}}}


#endif
