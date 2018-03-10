#ifndef TOOLKIT_NET_IPV4_SERVERSOCKET_H
#define TOOLKIT_NET_IPV4_SERVERSOCKET_H

#include <toolkit/net/bsd/Socket.h>
#include <toolkit/net/ISocket.h>

TOOLKIT_NS_BEGIN
namespace net { namespace ipv4
{

	struct Endpoint;

	class TCPServerSocket :
		protected bsd::Socket,
		public virtual ISocket
	{
	public:
		TCPServerSocket(int flags = ISocket::DefaultFlags): bsd::Socket(AF_INET, SOCK_STREAM, 0)
		{ SetNonBlocking(flags & ISocket::NonBlocking); }

		void Listen(const ipv4::Endpoint & ep, int backlogDepth = ISocket::DefaultBacklogDepth);
	};

}}
TOOLKIT_NS_END

#endif