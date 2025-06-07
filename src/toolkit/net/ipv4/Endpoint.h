#ifndef TOOLKIT_NET_IPV4_ENDPOINT_H
#define TOOLKIT_NET_IPV4_ENDPOINT_H

#include <toolkit/net/ipv4/Address.h>
#include <toolkit/core/Hash.h>
#include <netinet/in.h>

namespace TOOLKIT_NS
{

namespace net { namespace bsd
{
	class Socket;
}}

namespace net { namespace ipv4
{

	struct Endpoint
	{
		ipv4::Address	Address;
		int 			Port;

		Endpoint(const sockaddr & sockaddr);
		Endpoint(const sockaddr_in & sockaddr);
		Endpoint(ipv4::Address address, int port): Address(address), Port(port)
		{ }

		sockaddr_in GetAddress() const;

		void Connect(bsd::Socket & socket) const;
		void Bind(bsd::Socket & socket) const;

		bool operator==(const Endpoint &ep) const
		{ return Address == ep.Address && Port == ep.Port; }
		bool operator!=(const Endpoint &ep) const
		{ return !((*this) == ep); }

		bool operator<(const Endpoint &ep) const
		{
			if (Address != ep.Address)
				return Address < ep.Address;
			return Port < ep.Port;
		}

		struct Hash
		{
			size_t operator()(const Endpoint & ep) const
			{ return CombineHash(ep.Address, ep.Port); }
		};
	};

}}}


TOOLKIT_DECLARE_STD_HASH(TOOLKIT_NS ::net::ipv4::Endpoint, TOOLKIT_NS ::net::ipv4::Endpoint::Hash);


#endif
