#ifndef TOOLKIT_NET_IPV4_SOCKET_H
#define TOOLKIT_NET_IPV4_SOCKET_H

#include <toolkit/net/bsd/Socket.h>
#include <toolkit/net/ISocket.h>

TOOLKIT_SUBNS2_BEGIN(net, ipv4)

	class TCPSocket :
		protected bsd::Socket,
		public virtual ISocket
	{
	public:
		TCPSocket(int flags = ISocket::DefaultFlags): bsd::Socket(AF_INET, SOCK_STREAM, 0)
		{ SetNonBlocking(flags & ISocket::NonBlocking); }
	};

TOOLKIT_SUBNS2_END

#endif
