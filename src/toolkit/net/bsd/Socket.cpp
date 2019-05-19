#include <toolkit/net/bsd/Socket.h>
#include <toolkit/io/SystemException.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <fcntl.h>

namespace TOOLKIT_NS { namespace net { namespace bsd
{

	Socket::Socket(int family, int type, int proto): _socket(socket(family, type, proto)), _nonBlocking(false)
	{
		if (_socket == -1)
			throw io::SystemException("socket");
	}

	Socket::~Socket()
	{
		if (_socket >= 0)
			close(_socket);
	}

	void Socket::GetOption(int level, int optname, void *optval, socklen_t *optlen) const
	{ SYSTEM_CALL(getsockopt(_socket, level, optname, optval, optlen)); }

	void Socket::SetOption(int level, int optname, const void *optval, socklen_t optlen)
	{ SYSTEM_CALL(setsockopt(_socket, level, optname, optval, optlen)); }

	int Socket::GetFlags() const
	{
		int flags = fcntl(_socket, F_GETFL, 0);
		if (flags == -1)
			throw io::SystemException("fcntl(socketfd, F_GETFL, 0)");
		return flags;
	}

	void Socket::SetFlags(int flags)
	{ SYSTEM_CALL(fcntl(_socket, F_SETFL, flags)); }

	bool Socket::GetNonBlocking() const
	{ return GetFlags() & O_NONBLOCK; }

	void Socket::SetNonBlocking(bool enabled)
	{
		int flags = GetFlags();

		if (enabled)
			flags |= O_NONBLOCK;
		else
			flags &= ~O_NONBLOCK;

		SetFlags(flags);
		_nonBlocking = enabled;
	}

	void Socket::Connect(const struct sockaddr *addr, socklen_t addrlen)
	{ SYSTEM_CALL(connect(_socket, addr, addrlen)); }

	void Socket::Bind(const struct sockaddr *addr, socklen_t addrlen)
	{ SYSTEM_CALL(bind(_socket, addr, addrlen)); }

	void Socket::Listen(int backlog)
	{ SYSTEM_CALL(listen(_socket, backlog)); }

	int Socket::Accept(struct sockaddr *addr, socklen_t *addrlen)
	{ SYSTEM_CALL_RETURN(accept(_socket, addr, addrlen)); }

	ssize_t Socket::Send(const void *buf, size_t len, int flags)
	{ SYSTEM_CALL_RETURN(send(_socket, buf, len, flags)); }

	ssize_t Socket::SendTo(const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen)
	{ SYSTEM_CALL_RETURN(sendto(_socket, buf, len, flags, dest_addr, addrlen)); }

	ssize_t Socket::SendMsg(const struct msghdr *msg, int flags)
	{ SYSTEM_CALL_RETURN(sendmsg(_socket, msg, flags)); }

	ssize_t Socket::Recv(void *buf, size_t len, int flags)
	{ SYSTEM_CALL_RETURN(recv(_socket, buf, len, flags)); }

	ssize_t Socket::RecvFrom(void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen)
	{ SYSTEM_CALL_RETURN(recvfrom(_socket, buf, len, flags, src_addr, addrlen)); }

	ssize_t Socket::RecvMsg(struct msghdr *msg, int flags)
	{ SYSTEM_CALL_RETURN(recvmsg(_socket, msg, flags)); }

	ssize_t Socket::SendFile(int in_fd, off_t *offset, size_t count)
	{ SYSTEM_CALL_RETURN(sendfile(_socket, in_fd, offset, count)); }

	void Socket::Shutdown(int how)
	{ shutdown(_socket, how); }

}}}

