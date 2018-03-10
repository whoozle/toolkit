#include <toolkit/net/ipv4/TCPServerSocket.h>
#include <toolkit/net/ipv4/Endpoint.h>

TOOLKIT_NS_BEGIN
namespace net { namespace ipv4
{

	void TCPServerSocket::Listen(const ipv4::Endpoint & ep, int backlogDepth)
	{
		ep.Bind(*this);
		bsd::Socket::Listen(backlogDepth);
	}

}}
TOOLKIT_NS_END
