#ifndef TOOLKIT_NET_UNIX_LOCALSOCKET_H
#define TOOLKIT_NET_UNIX_LOCALSOCKET_H

#include <toolkit/net/BaseSocket.h>

namespace TOOLKIT_NS { namespace net { namespace unix
{
	class LocalSocket : public BaseSocket
	{
	public:
		LocalSocket(int flags = ISocket::DefaultFlags): BaseSocket(AF_UNIX, SOCK_STREAM, 0, flags)
		{ SetNonBlocking(flags & ISocket::NonBlocking); }

	};
}}}


#endif
