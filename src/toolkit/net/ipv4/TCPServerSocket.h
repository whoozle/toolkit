#ifndef TOOLKIT_NET_IPV4_TCPSERVERSOCKET_H
#define TOOLKIT_NET_IPV4_TCPSERVERSOCKET_H

#include <toolkit/net/ipv4/TCPSocket.h>
#include <toolkit/net/BaseServerSocket.h>

namespace TOOLKIT_NS { namespace net { namespace ipv4
{

	class TCPServerSocket :
		public BaseServerSocket<TCPSocket>
	{
	public:
		using BaseServerSocket::BaseServerSocket;
	};

}}}


#endif
