#include <toolkit/net/ipv4/Endpoint.h>
#include <toolkit/net/ipv4/TCPServerSocket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

namespace TOOLKIT_NS { namespace net { namespace ipv4
{
	void Endpoint::Connect(bsd::Socket & socket) const
	{
		struct sockaddr_in address = { };
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = Address.GetNetworkAddress();
		address.sin_port = htons(Port);
		socket.Connect(reinterpret_cast<struct sockaddr *>(&address), sizeof(address));
	}

	void Endpoint::Bind(bsd::Socket & socket) const
	{
		struct sockaddr_in address = { };
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = Address.GetNetworkAddress();
		address.sin_port = htons(Port);
		socket.Bind(reinterpret_cast<struct sockaddr *>(&address), sizeof(address));
	}

}}}

