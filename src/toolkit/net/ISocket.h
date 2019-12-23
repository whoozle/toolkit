#ifndef TOOLKIT_NET_ISOCKET_H
#define TOOLKIT_NET_ISOCKET_H

#include <toolkit/io/IPollable.h>
#include <toolkit/core/types.h>
#include <memory>

namespace TOOLKIT_NS { namespace net
{

	struct ISocket : public io::IPollable
	{
		static constexpr int DefaultFlags = 0;
		static constexpr int NonBlocking = 1;

		virtual ~ISocket() = default;
	};
	TOOLKIT_DECLARE_PTR(ISocket);

}}


#endif

