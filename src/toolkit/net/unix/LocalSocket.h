#ifndef TOOLKIT_NET_UNIX_LOCALSOCKET_H
#define TOOLKIT_NET_UNIX_LOCALSOCKET_H

#include <toolkit/net/BaseSocket.h>
#include <sys/socket.h>

namespace TOOLKIT_NS { namespace net { namespace unix
{
	class LocalSocket : public BaseSocket
	{
	public:
		LocalSocket(): BaseSocket(AF_UNIX, SOCK_STREAM, 0)
		{ }

		LocalSocket(BaseSocket && o): BaseSocket(std::move(o))
		{ }

		struct ucred GetPeerCredentials() const
		{
			struct ucred creds = {};
			socklen_t size = sizeof(creds);
			GetOption(SOL_SOCKET, SO_PEERCRED, &creds, &size);
			return creds;
		}
	};
}}}


#endif
