#ifndef TOOLKIT_NET_IPV4_UNIXSERVERSOCKET_H
#define TOOLKIT_NET_IPV4_UNIXSERVERSOCKET_H

#include <toolkit/net/ipv4/TCPSocket.h>
#include <toolkit/net/ipv4/BaseServerSocket.h>

TOOLKIT_NS_BEGIN
namespace net { namespace ipv4
{

	class TCPServerSocket :
		public BaseServerSocket<UnixSocket>
	{

	};

}}
TOOLKIT_NS_END

#endif
