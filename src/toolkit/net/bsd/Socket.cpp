#include <toolkit/net/bsd/Socket.h>
#include <toolkit/io/SystemException.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

TOOLKIT_SUBNS2_BEGIN(net, bsd)

	Socket::Socket(int family, int type, int proto): _socket(socket(family, type, proto))
	{
		if (_socket == -1)
			throw io::SystemException("socket");
	}

	Socket::~Socket()
	{
		close(_socket);
	}

	void Socket::GetOption(int level, int optname, void *optval, socklen_t *optlen)
	{
		SYSTEM_CALL(getsockopt(_socket, level, optname, optval, optlen));
	}

	void Socket::SetOption(int level, int optname, const void *optval, socklen_t optlen)
	{
		SYSTEM_CALL(setsockopt(_socket, level, optname, optval, optlen));
	}


TOOLKIT_SUBNS2_END
