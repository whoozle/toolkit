#ifndef TOOLKIT_NET_IPV4_SOCKET_H
#define TOOLKIT_NET_IPV4_SOCKET_H

#include <toolkit/net/bsd/Socket.h>

TOOLKIT_SUBNS2_BEGIN(net, ipv4)

	class TCPSocket : public bsd::Socket
	{
	public:
		TCPSocket(): bsd::Socket(AF_INET, SOCK_STREAM, 0)
		{ }
	};

TOOLKIT_SUBNS2_END

#endif
