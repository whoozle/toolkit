#ifndef TOOLKIT_NET_IPV4_ADDRESS_H
#define TOOLKIT_NET_IPV4_ADDRESS_H

#include <toolkit/core/types.h>
#include <string>

namespace TOOLKIT_NS { namespace net { namespace ipv4
{

	class Address
	{
		u32		_addr;

	public:
		Address(u32 addr): _addr(addr)
		{ }

		Address(unsigned a, unsigned b, unsigned c, unsigned d)
		{ _addr = (d << 24) | (c << 16) | (b << 8) | a; }

		u32 GetNetworkAddress() const
		{ return _addr; }

		static Address Any();
		static Address Broadcast();
		static Address None();
		static Address Localhost();
		static Address FromString(const std::string &addr);
	};

}}}


#endif
