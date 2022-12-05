#ifndef TOOLKIT_NET_UNIX_LOCALSOCKET_H
#define TOOLKIT_NET_UNIX_LOCALSOCKET_H

#include <toolkit/net/BaseSocket.h>
#include <sys/socket.h>

namespace TOOLKIT_NS { namespace net { namespace unix
{
	template<int SocketType>
	class BaseLocalSocket : public BaseSocket
	{
	public:
		using BaseSocket::BaseSocket;

		BaseLocalSocket(): BaseSocket(AF_UNIX, SocketType, 0)
		{ }

		struct ucred GetPeerCredentials() const
		{
			struct ucred creds = {};
			socklen_t size = sizeof(creds);
			GetOption(SOL_SOCKET, SO_PEERCRED, &creds, &size);
			return creds;
		}
	};

	class LocalSocket : public BaseLocalSocket<SOCK_STREAM>
	{
	public:
		using BaseLocalSocket::BaseLocalSocket;
	};

	class LocalDatagramSocket : public BaseLocalSocket<SOCK_DGRAM>
	{
	public:
		using BaseLocalSocket::BaseLocalSocket;
	};
}}}


#endif
