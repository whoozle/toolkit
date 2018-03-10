#include <toolkit/net/ipv4/TCPServerSocket.h>
#include <toolkit/net/ipv4/TCPSocket.h>
#include <toolkit/net/ipv4/Endpoint.h>

TOOLKIT_NS_BEGIN
namespace net { namespace ipv4
{

	void TCPServerSocket::Listen(const ipv4::Endpoint & ep, int backlogDepth)
	{
		ep.Bind(*this);
		bsd::Socket::Listen(backlogDepth);
	}

	TCPSocket * TCPServerSocket::Accept()
	{
		struct sockaddr addr;
		socklen_t addrlen;

		return new TCPSocket(bsd::Socket::Accept(&addr, &addrlen));
	}

}}
TOOLKIT_NS_END
