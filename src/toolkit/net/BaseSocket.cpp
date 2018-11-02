#include <toolkit/net/ipv4/TCPSocket.h>
#include <toolkit/net/ipv4/Endpoint.h>

TOOLKIT_NS_BEGIN
namespace net
{

	BaseSocket::~BaseSocket()
	{
		bsd::Socket::Shutdown(SHUT_RDWR);
	}

	ssize_t BaseSocket::Write(io::ConstByteData data)
	{
		return bsd::Socket::Send(data.data(), data.size(), 0);
	}


}
TOOLKIT_NS_END
