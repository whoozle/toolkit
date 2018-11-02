#ifndef TOOLKIT_NET_IPV4_UNIXSOCKET_H
#define TOOLKIT_NET_IPV4_UNIXSOCKET_H

#include <toolkit/net/BaseSocket.h>

TOOLKIT_NS_BEGIN
namespace net
{
	struct Endpoint;

	class TCPSocket : public BaseSocket
	{
	public:
		TCPSocket(int flags = ISocket::DefaultFlags): BaseSocket(AF_INET, SOCK_STREAM, 0, flags)
		{ SetNonBlocking(flags & ISocket::NonBlocking); }
	};

}
TOOLKIT_NS_END

#endif
