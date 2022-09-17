#include <toolkit/net/ipv4/TCPSocket.h>
#include <toolkit/net/ipv4/Endpoint.h>

namespace TOOLKIT_NS { namespace net
{

	BaseSocket::~BaseSocket()
	{
		try { bsd::Socket::Shutdown(SHUT_RDWR); }
		catch(const std::exception &) { }
	}

}}

