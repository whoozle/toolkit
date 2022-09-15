#ifndef TOOLKIT_NET_ISOCKET_H
#define TOOLKIT_NET_ISOCKET_H

#include <toolkit/io/IPollable.h>
#include <toolkit/core/Buffer.h>
#include <toolkit/core/types.h>
#include <memory>

namespace TOOLKIT_NS { namespace net
{

	struct ISocket : public virtual io::IPollable
	{
		virtual ~ISocket() = default;

		virtual ssize_t Send(ConstBuffer data, int flags) = 0;
		virtual ssize_t Recv(Buffer data, int flags) = 0;
	};
	TOOLKIT_DECLARE_PTR(ISocket);

}}


#endif

