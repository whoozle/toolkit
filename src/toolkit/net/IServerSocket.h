#ifndef TOOLKIT_NET_ISERVERSOCKET_H
#define TOOLKIT_NET_ISERVERSOCKET_H

#include <toolkit/core/types.h>
#include <toolkit/io/IPollable.h>
#include <memory>

namespace TOOLKIT_NS { namespace net
{
	struct ISocket;

	struct IServerSocket : public virtual io::IPollable
	{
		static constexpr int DefaultBacklogDepth = 128;

		virtual ~IServerSocket() = default;

		virtual ISocket * Accept() = 0;
	};
	TOOLKIT_DECLARE_PTR(IServerSocket);

}}


#endif

