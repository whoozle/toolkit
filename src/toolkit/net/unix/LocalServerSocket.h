#ifndef TOOLKIT_NET_UNIX_LOCALSERVERSOCKET_H
#define TOOLKIT_NET_UNIX_LOCALSERVERSOCKET_H

#include <toolkit/net/unix/LocalSocket.h>
#include <toolkit/net/BaseServerSocket.h>

namespace TOOLKIT_NS { namespace net { namespace unix
{

	class LocalServerSocket :
		public BaseServerSocket<LocalSocket>
	{
		using super = BaseServerSocket<LocalSocket>;
	public:
		LocalServerSocket(int flags = ISocket::DefaultFlags):
			super(AF_UNIX, SOCK_STREAM, 0, flags)
		{ }
	};

}}}


#endif
