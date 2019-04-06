#include <toolkit/net/ipv4/TCPSocket.h>
#include <toolkit/net/ipv4/Endpoint.h>

namespace TOOLKIT_NS { namespace net
{

	BaseSocket::~BaseSocket()
	{
		bsd::Socket::Shutdown(SHUT_RDWR);
	}

	ssize_t BaseSocket::Write(ConstBuffer data)
	{
		return bsd::Socket::Send(data.data(), data.size(), 0);
	}


}}

