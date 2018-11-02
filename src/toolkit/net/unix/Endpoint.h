#ifndef TOOLKIT_NET_UNIX_ENDPOINT_H
#define TOOLKIT_NET_UNIX_ENDPOINT_H

#include <toolkit/core/core.h>
#include <string>

TOOLKIT_NS_BEGIN

namespace net { namespace bsd
{
	class Socket;
}}

namespace net { namespace unix
{

	struct Endpoint
	{
		std::string Path;

		Endpoint(const std::string &path): Path(path)
		{ }

		void Connect(bsd::Socket & socket) const;
		void Bind(bsd::Socket & socket) const;
	};

}}
TOOLKIT_NS_END

#endif
