#ifndef TOOLKIT_NET_UNIX_LOCALSERVERSOCKET_H
#define TOOLKIT_NET_UNIX_LOCALSERVERSOCKET_H

#include <toolkit/net/unix/LocalSocket.h>
#include <toolkit/net/BaseServerSocket.h>

TOOLKIT_NS_BEGIN
namespace net { namespace unix
{

	class LocalServerSocket :
		public BaseServerSocket<LocalSocket>
	{
	public:
		using BaseServerSocket::BaseServerSocket;
	};

}}
TOOLKIT_NS_END

#endif
