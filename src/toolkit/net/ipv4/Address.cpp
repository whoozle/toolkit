#include <toolkit/net/ipv4/Address.h>
#include <toolkit/io/SystemException.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

TOOLKIT_SUBNS2_BEGIN(net, ipv4)

	Address Address::FromString(const std::string &addr)
	{
		struct in_addr inp;
		if (inet_aton(addr.c_str(), &inp) != 1)
			throw io::SystemException("Address::FromString");
		return Address(inp.s_addr);
	}

TOOLKIT_SUBNS2_END
