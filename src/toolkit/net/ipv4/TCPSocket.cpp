#include <toolkit/net/ipv4/TCPSocket.h>
#include <toolkit/net/ipv4/Endpoint.h>

TOOLKIT_NS_BEGIN
namespace net { namespace ipv4
{

	void TCPSocket::Connect(const Endpoint & ep)
	{
		ep.Connect(*this);
	}

}}
TOOLKIT_NS_END
