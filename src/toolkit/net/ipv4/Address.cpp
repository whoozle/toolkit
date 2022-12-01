#include <toolkit/net/ipv4/Address.h>
#include <toolkit/io/SystemException.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace TOOLKIT_NS { namespace net { namespace ipv4
{

	Address Address::Any()
	{ return Address(htonl(INADDR_ANY)); }

	Address Address::Broadcast()
	{ return Address(htonl(INADDR_BROADCAST)); }

	Address Address::None()
	{ return Address(htonl(INADDR_NONE)); }

	Address Address::Localhost()
	{ return Address(htonl(INADDR_LOOPBACK)); }

	Address Address::FromString(const std::string &addr)
	{
		struct in_addr inp;
		int r = inet_aton(addr.c_str(), &inp);
		ASSERT(r >= 0, io::SystemException, "Address::FromString");
		if (r == 0)
			THROW(Exception, "inet_aton: parse error: " + addr);
		return Address(inp.s_addr);
	}

}}}

