#ifndef TOOLKIT_NET_ISOCKET_H
#define TOOLKIT_NET_ISOCKET_H

#include <toolkit/io/IPollable.h>
#include <toolkit/io/IStream.h>
#include <toolkit/core/Buffer.h>
#include <toolkit/core/types.h>
#include <memory>

namespace TOOLKIT_NS { namespace net
{

	struct ISocket : public virtual io::IPollable
	{
		virtual ~ISocket() = default;

		virtual size_t Send(ConstBuffer data, int flags) = 0;
		virtual size_t Recv(Buffer data, int flags) = 0;
	};
	TOOLKIT_DECLARE_PTR(ISocket);

	struct ISocketStream :
		public virtual ISocket,
		public virtual io::IBidirectionalStream
	{
		virtual int GetReadFlags() const
		{ return 0; }

		virtual int GetWriteFlags() const
		{ return 0; }

		virtual size_t Read(Buffer data) override
		{ return Recv(data, GetReadFlags()); }

		virtual size_t Write(ConstBuffer data) override
		{ return Send(data, GetWriteFlags()); }
	};

}}


#endif

