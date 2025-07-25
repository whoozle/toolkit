#include <toolkit/net/ipv4/Endpoint.h>
#include <toolkit/net/ipv4/TCPServerSocket.h>
#include <toolkit/io/SystemException.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

namespace TOOLKIT_NS { namespace net { namespace ipv4
{
	Endpoint::Endpoint(const sockaddr & sockaddr): Endpoint(reinterpret_cast<const sockaddr_in &>(sockaddr))
	{ ASSERT (sockaddr.sa_family == AF_INET, Exception, "Invalid sockaddr family, expected AF_INET"); }

	Endpoint::Endpoint(const sockaddr_in & in)
	{
		Address = ipv4::Address(in.sin_addr.s_addr);
		Port = ntohs(in.sin_port);
	}

	sockaddr_in Endpoint::GetAddress() const
	{
		struct sockaddr_in address = { };
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = Address.GetNetworkAddress();
		address.sin_port = htons(Port);
		return address;
	}

	void Endpoint::Connect(bsd::Socket & socket) const
	{
		auto address = GetAddress();
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

	void Endpoint::ToString(text::StringOutputStream & ss) const
	{
		auto address = Address.GetNetworkAddress();
		char buf[64];
		ASSERT(inet_ntop(AF_INET, &address, buf, sizeof(buf)), io::SystemException, "inet_ntop");
		ss << buf << ':' << Port;
	}

}}}

