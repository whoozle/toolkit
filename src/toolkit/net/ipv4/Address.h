#ifndef TOOLKIT_NET_IPV4_ADDRESS_H
#define TOOLKIT_NET_IPV4_ADDRESS_H

#include <toolkit/core/types.h>
#include <toolkit/core/Hash.h>
#include <string>

namespace TOOLKIT_NS { namespace net { namespace ipv4
{

	class Address
	{
		u32		_addr;

	public:
		explicit Address(u32 addr = 0): _addr(addr)
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

		bool operator==(const Address &addr) const
		{ return _addr == addr._addr; }
		bool operator!=(const Address &addr) const
		{ return !((*this) == addr); }
		bool operator<(const Address &o) const
		{ return _addr < o._addr; }

		struct Hash
		{
			size_t operator()(const Address & addr) const
			{ return addr.GetNetworkAddress(); }
		};

	};

}}}


TOOLKIT_DECLARE_STD_HASH(TOOLKIT_NS ::net::ipv4::Address, TOOLKIT_NS ::net::ipv4::Address::Hash);

#endif
