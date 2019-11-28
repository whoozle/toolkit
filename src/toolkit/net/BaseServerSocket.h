#ifndef TOOLKIT_NET_BASESERVERSOCKET_H
#define TOOLKIT_NET_BASESERVERSOCKET_H

#include <toolkit/net/bsd/Socket.h>
#include <toolkit/net/ISocket.h>

namespace TOOLKIT_NS { namespace net
{

	struct Endpoint;

	template<typename SocketType>
	class BaseServerSocket :
		protected bsd::Socket,
		public virtual ISocket
	{
	public:
		BaseServerSocket(int family, int type, int proto, int flags = ISocket::DefaultFlags): bsd::Socket(family, type, proto)
		{ SetNonBlocking(flags & ISocket::NonBlocking); }

		template<typename EndpointType>
		void Listen(const EndpointType & ep, int backlogDepth = ISocket::DefaultBacklogDepth)
		{
			ep.Bind(*this);
			bsd::Socket::Listen(backlogDepth);
		}

		using bsd::Socket::SetNonBlocking;
		using bsd::Socket::GetNonBlocking;
		using bsd::Socket::SetOption;
		using bsd::Socket::GetOption;

		SocketType * Accept()
		{
			struct sockaddr addr;
			socklen_t addrlen = sizeof(addr);

			return new SocketType(bsd::Socket::Accept(&addr, &addrlen));
		}
	};

}}


#endif
