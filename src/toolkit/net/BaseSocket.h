#ifndef TOOLKIT_NET_IPV4_BASESOCKET_H
#define TOOLKIT_NET_IPV4_BASESOCKET_H

#include <toolkit/net/bsd/Socket.h>
#include <toolkit/net/ISocket.h>
#include <toolkit/core/Buffer.h>

TOOLKIT_NS_BEGIN
namespace net
{

	class BaseSocket :
		protected bsd::Socket,
		public virtual io::IPollable,
		public virtual ISocket
	{
	public:
		BaseSocket(int family, int type, int proto, int flags = ISocket::DefaultFlags): bsd::Socket(family, type, proto)
		{ SetNonBlocking(flags & ISocket::NonBlocking); }
		~BaseSocket();

		using bsd::Socket::SetNonBlocking;
		using bsd::Socket::GetNonBlocking;

		template<typename EndpointType>
		void Connect(const EndpointType & ep)
		{ ep.Connect(*this); }

		ssize_t Write(ConstBuffer data);
	};

}
TOOLKIT_NS_END

#endif
