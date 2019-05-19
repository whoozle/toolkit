#ifndef TOOLKIT_NET_UNIX_ENDPOINT_H
#define TOOLKIT_NET_UNIX_ENDPOINT_H

#include <toolkit/core/core.h>
#include <string>

namespace TOOLKIT_NS
{

namespace net { namespace bsd
{
	class Socket;
}}

namespace net { namespace unix
{

	struct Endpoint
	{
		std::string Path;

		Endpoint(const std::string &path, bool abstract = true): Path(abstract? std::string(1, '\0') + path: path)
		{ }

		void Connect(bsd::Socket & socket) const;
		void Bind(bsd::Socket & socket) const;
	};

}}}


#endif
