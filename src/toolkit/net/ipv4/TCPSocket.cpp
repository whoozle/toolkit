#include <toolkit/net/ipv4/TCPSocket.h>
#include <toolkit/net/ipv4/Endpoint.h>

TOOLKIT_NS_BEGIN
namespace net { namespace ipv4
{

	void TCPSocket::Connect(const Endpoint & ep)
	{
		ep.Connect(*this);
	}

	TCPSocket::~TCPSocket()
	{
		bsd::Socket::Shutdown(SHUT_RDWR);
	}

}}
TOOLKIT_NS_END
