#ifndef TOOLKIT_NET_ISOCKET_H
#define TOOLKIT_NET_ISOCKET_H

TOOLKIT_NS_BEGIN
namespace net
{

	struct ISocket
	{
		static constexpr int DefaultFlags = 0;
		static constexpr int NonBlocking = 1;

		virtual ~ISocket() = default;
	};

}
TOOLKIT_NS_END

#endif

