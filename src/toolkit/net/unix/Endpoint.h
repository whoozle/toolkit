#ifndef TOOLKIT_NET_UNIX_ENDPOINT_H
#define TOOLKIT_NET_UNIX_ENDPOINT_H

#include <toolkit/core/core.h>
#include <string>
#include <sys/un.h>

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
		bool		Abstract;

		Endpoint(const std::string &path, bool abstract = true):
			Path(path), Abstract(abstract)
		{ }

		sockaddr_un GetAddress() const;

		void Connect(bsd::Socket & socket) const;
		void Bind(bsd::Socket & socket) const;
	};

}}}


#endif
