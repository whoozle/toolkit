#include <toolkit/net/unix/Endpoint.h>
#include <toolkit/net/bsd/Socket.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace TOOLKIT_NS { namespace net { namespace unix
{
	void Endpoint::Connect(bsd::Socket & socket) const
	{
		struct sockaddr_un address = { };
		address.sun_family = AF_UNIX;
		strncpy(address.sun_path, Path.c_str(), sizeof(address.sun_path) - 1);
		address.sun_path[sizeof(address.sun_path) - 1] = 0;
		socket.Connect(reinterpret_cast<struct sockaddr *>(&address), sizeof(address));
	}

	void Endpoint::Bind(bsd::Socket & socket) const
	{
		struct sockaddr_un address = { };
		address.sun_family = AF_UNIX;
		strncpy(address.sun_path, Path.c_str(), sizeof(address.sun_path) - 1);
		address.sun_path[sizeof(address.sun_path) - 1] = 0;
		socket.Bind(reinterpret_cast<struct sockaddr *>(&address), sizeof(address));
	}

}}}

