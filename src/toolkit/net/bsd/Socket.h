#ifndef TOOLKIT_NET_BSD_SOCKET_H
#define TOOLKIT_NET_BSD_SOCKET_H

#include <toolkit/core/core.h>
#include <toolkit/core/Buffer.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/net/ISocket.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace TOOLKIT_NS { namespace net { namespace bsd
{

	class Socket :
		public virtual net::ISocket,
		Noncopyable
	{
		int		_socket;
		bool	_nonBlocking;

	public:
		Socket(int family, int type, int proto);

		Socket(int fd): _socket(fd), _nonBlocking(false)
		{ }

		Socket(Socket && socket) noexcept:
			_socket(socket._socket),
			_nonBlocking(socket._nonBlocking)
		{ socket._socket = -1; }
		~Socket();

		int GetFileDescriptor() const override
		{ return _socket; }

		void GetOption(int level, int optname, void *optval, socklen_t *optlen) const;
		void SetOption(int level, int optname, const void *optval, socklen_t optlen);

		int GetFlags() const;
		void SetFlags(int flags);

		bool GetNonBlocking() const;
		void SetNonBlocking(bool enabled);

		void GetName(struct sockaddr *addr, socklen_t *addrlen);
		void Connect(const struct sockaddr *addr, socklen_t addrlen);
		void Bind(const struct sockaddr *addr, socklen_t addrlen);
		void Listen(int backlog);
		int Accept(struct sockaddr *addr, socklen_t *addrlen);

		size_t Send(ConstBuffer data, int flags) override;
		size_t SendTo(ConstBuffer data, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);
		size_t SendMsg(const struct msghdr *msg, int flags);

		size_t Recv(Buffer data, int flags) override;
		size_t RecvFrom(Buffer data, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen);
		size_t RecvMsg(struct msghdr *msg, int flags);

		size_t SendFile(int in_fd, off_t *offset, size_t count);

		void Shutdown(int how);
	};
	TOOLKIT_DECLARE_PTR(Socket);

}}}


#endif

