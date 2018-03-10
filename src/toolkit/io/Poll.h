#ifndef TOOLKIT_IO_POLL_H
#define TOOLKIT_IO_POLL_H

#include <toolkit/core/types.h>
#include <toolkit/io/IPollable.h>

TOOLKIT_NS_BEGIN
namespace io
{

	class Poll
	{
		int _fd;

	public:
		struct Event
		{
			IPollable *	Pollable;
			uint		Events;
		};

		static constexpr uint EventInput			= 1 << 0;
		static constexpr uint EventOutput			= 1 << 1;
		static constexpr uint EventHangup			= 1 << 2;
		static constexpr uint EventError			= 1 << 3;
		static constexpr uint EventPriority			= 1 << 4;
		static constexpr uint EventEdgeTriggered	= 1 << 5;

		Poll();
		~Poll();

		void Add(const IPollable & pollable, int events);
		void Modify(const IPollable & pollable, int events);
		void Remove(const IPollable & pollable, int events);

		size_t Wait(Event *events, size_t eventsMax, int timeout);
	};

}
TOOLKIT_NS_END

#endif
