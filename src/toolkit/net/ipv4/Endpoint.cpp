#include <toolkit/net/ipv4/Endpoint.h>
#include <toolkit/net/ipv4/TCPServerSocket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

TOOLKIT_NS_BEGIN
namespace net { namespace ipv4
{

	void Endpoint::Bind(bsd::Socket & socket) const
	{
		struct sockaddr_in address = { };
		address.sin_family = AF_INET;
		socket.Bind((struct sockaddr *)&address, sizeof(address));
	}

}}
TOOLKIT_NS_END
