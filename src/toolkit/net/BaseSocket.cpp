#include <toolkit/net/ipv4/TCPSocket.h>
#include <toolkit/net/ipv4/Endpoint.h>

namespace TOOLKIT_NS { namespace net
{

	BaseSocket::~BaseSocket()
	{
		bsd::Socket::Shutdown(SHUT_RDWR);
	}

	ssize_t BaseSocket::Write(ConstBuffer data)
	{ return bsd::Socket::Send(data, 0); }

	ssize_t BaseSocket::Read(Buffer data)
	{ return bsd::Socket::Recv(data, 0); }


}}

