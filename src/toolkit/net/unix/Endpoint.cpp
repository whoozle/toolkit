#include <toolkit/net/unix/Endpoint.h>
#include <toolkit/net/bsd/Socket.h>
#include <toolkit/log/Logger.h>
#include <sys/socket.h>
#include <sys/un.h>

namespace TOOLKIT_NS { namespace net { namespace unix
{
	sockaddr_un Endpoint::GetAddress() const
	{
		struct sockaddr_un address = { };
		address.sun_family = AF_UNIX;
		size_t offset = Abstract? 1: 0;
		size_t maxSize = sizeof(address.sun_path) - (Abstract? 2: 1);
		strncpy(address.sun_path + offset, Path.c_str(), maxSize);
		address.sun_path[sizeof(address.sun_path) - 1] = 0;
		return address;
	}

	void Endpoint::Connect(bsd::Socket & socket) const
	{
		auto address = GetAddress();
		socket.Connect(reinterpret_cast<struct sockaddr *>(&address), sizeof(address));
	}

	void Endpoint::Bind(bsd::Socket & socket) const
	{
		auto address = GetAddress();
		socket.Bind(reinterpret_cast<struct sockaddr *>(&address), sizeof(address));
	}

}}}

