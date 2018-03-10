#ifndef TOOLKIT_NET_ISOCKET_H
#define TOOLKIT_NET_ISOCKET_H

TOOLKIT_SUBNS_BEGIN(net)

	struct ISocket
	{
		static constexpr int DefaultFlags = 0;
		static constexpr int NonBlocking = 1;

		virtual ~ISocket() = default;
	};

TOOLKIT_SUBNS_END

#endif

