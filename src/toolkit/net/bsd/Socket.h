#ifndef TOOLKIT_NET_BSD_SOCKET_H
#define TOOLKIT_NET_BSD_SOCKET_H

#include <toolkit/core/core.h>
#include <sys/socket.h>

TOOLKIT_SUBNS2_BEGIN(net, bsd)

	class Socket
	{
		int _socket;

	public:
		Socket(int family, int type, int proto);
		~Socket();

		void GetOption(int level, int optname, void *optval, socklen_t *optlen);
		void SetOption(int level, int optname, const void *optval, socklen_t optlen);
	};

TOOLKIT_SUBNS2_END

#endif

