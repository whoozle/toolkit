#include <toolkit/net/unix/Enpoint.h>
#include <sys/socket.h>
#include <netinet/un.h>

TOOLKIT_NS_BEGIN
namespace net { namespace unix
{
	void Endpoint::Connect(bsd::Socket & socket) const
	{
		struct sockaddr_un address = { };
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = Address.GetNetworkAddress();
		address.sin_port = htons(Port);
		socket.Connect((struct sockaddr *)&address, sizeof(address));
	}

	void Endpoint::Bind(bsd::Socket & socket) const
	{
		struct sockaddr_un address = { };
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = Address.GetNetworkAddress();
		address.sin_port = htons(Port);
		socket.Bind((struct sockaddr *)&address, sizeof(address));
	}

}}
TOOLKIT_NS_END
