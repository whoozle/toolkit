#ifndef TOOLKIT_NET_BASESERVERSOCKET_H
#define TOOLKIT_NET_BASESERVERSOCKET_H

#include <toolkit/net/bsd/Socket.h>
#include <toolkit/net/ISocket.h>

TOOLKIT_NS_BEGIN
namespace net
{

	struct Endpoint;

	template<typename SocketType>
	class BaseServerSocket :
		protected bsd::Socket,
		public virtual io::IPollable,
		public virtual ISocket
	{
	public:
		BaseServerSocket(int flags = ISocket::DefaultFlags): bsd::Socket(AF_INET, SOCK_STREAM, 0)
		{ SetNonBlocking(flags & ISocket::NonBlocking); }

		template<typename EndpointType>
		void Listen(const EndpointType & ep, int backlogDepth = ISocket::DefaultBacklogDepth)
		{
			ep.Bind(*this);
			bsd::Socket::Listen(backlogDepth);
		}

		using bsd::Socket::SetNonBlocking;
		using bsd::Socket::GetNonBlocking;

		SocketType * Accept()
		{
			struct sockaddr addr;
			socklen_t addrlen = sizeof(addr);

			return new SocketType(bsd::Socket::Accept(&addr, &addrlen));
		}
	};

}
TOOLKIT_NS_END

#endif
