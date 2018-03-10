#ifndef TOOLKIT_NET_IPV4_ADDRESS_H
#define TOOLKIT_NET_IPV4_ADDRESS_H

#include <toolkit/core/types.h>
#include <string>

TOOLKIT_NS_BEGIN
namespace net { namespace ipv4
{

	class Address
	{
		u32		_addr;

	public:
		Address(u32 addr): _addr(addr)
		{ }

		Address(int a, int b, int c, int d)
		{ _addr = (a << 24) | (b << 16) | (c << 8) | d; }

		u32 GetNetworkAddress() const
		{ return _addr; }

		static Address Any();
		static Address Broadcast();
		static Address None();
		static Address Localhost();
		static Address FromString(const std::string &addr);
	};

}}
TOOLKIT_NS_END

#endif
