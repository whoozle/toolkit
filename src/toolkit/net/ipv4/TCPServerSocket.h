#ifndef TOOLKIT_NET_IPV4_SERVERSOCKET_H
#define TOOLKIT_NET_IPV4_SERVERSOCKET_H

#include <toolkit/net/bsd/Socket.h>
#include <toolkit/net/ISocket.h>

TOOLKIT_SUBNS2_BEGIN(net, ipv4)

	struct Endpoint;

	class TCPServerSocket :
		protected bsd::Socket,
		public virtual ISocket
	{
	public:
		TCPServerSocket(int flags = ISocket::DefaultFlags): bsd::Socket(AF_INET, SOCK_STREAM, 0)
		{ SetNonBlocking(flags & ISocket::NonBlocking); }

		void Bind(ipv4::Endpoint ep);
	};

TOOLKIT_SUBNS2_END

#endif
