#ifndef TOOLKIT_NET_IPV4_BASESOCKET_H
#define TOOLKIT_NET_IPV4_BASESOCKET_H

#include <toolkit/net/bsd/Socket.h>
#include <toolkit/net/ISocket.h>
#include <toolkit/core/Buffer.h>

namespace TOOLKIT_NS { namespace net
{

	class BaseSocket :
		public bsd::Socket,
		public virtual ISocket
	{
	public:
		using bsd::Socket::Socket;
		BaseSocket(BaseSocket && o): bsd::Socket(std::move(o))
		{ }
		~BaseSocket();

		template<typename EndpointType>
		void Connect(const EndpointType & ep)
		{ ep.Connect(*this); }

		ssize_t Write(ConstBuffer data);
		ssize_t Read(Buffer data);
	};

}}


#endif
