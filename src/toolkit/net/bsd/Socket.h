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

		int GetFlags() const;
		void SetFlags(int flags);

		bool GetNonBlocking() const;
		void SetNonBlocking(bool enabled);

		void Connect(const struct sockaddr *addr, socklen_t addrlen);
		void Bind(const struct sockaddr *addr, socklen_t addrlen);
		void Listen(int backlog);

		ssize_t Send(const void *buf, size_t len, int flags);
		ssize_t SendTo(const void *buf, size_t len, int flags,
                      const struct sockaddr *dest_addr, socklen_t addrlen);
		ssize_t SendMsg(const struct msghdr *msg, int flags);

		ssize_t Recv(void *buf, size_t len, int flags);
		ssize_t RecvFrom(void *buf, size_t len, int flags,
                        struct sockaddr *src_addr, socklen_t *addrlen);
		ssize_t RecvMsg(struct msghdr *msg, int flags);

		ssize_t SendFile(int in_fd, off_t *offset, size_t count);
	};

TOOLKIT_SUBNS2_END

#endif

