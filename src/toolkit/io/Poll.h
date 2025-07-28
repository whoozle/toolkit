#ifndef TOOLKIT_IO_POLL_H
#define TOOLKIT_IO_POLL_H

#include <toolkit/core/types.h>
#include <toolkit/core/Noncopyable.h>
#include <memory>

namespace TOOLKIT_NS { namespace io
{
	struct IPollable;
	struct IPollEventHandler;

	class Poll : Noncopyable
	{
		int _fd;

	public:
		static constexpr size_t MaxEvents			= 1024;

		static constexpr uint EventInput			= 1 << 0;
		static constexpr uint EventOutput			= 1 << 1;
		static constexpr uint EventHangup			= 1 << 2;
		static constexpr uint EventError			= 1 << 3;
		static constexpr uint EventPriority			= 1 << 4;
		static constexpr uint EventEdgeTriggered	= 1 << 5;

		Poll();
		~Poll();

		void Add(const IPollable & pollable, IPollEventHandler & handler, int events);
		void Modify(const IPollable & pollable, IPollEventHandler & handler, int events);
		void Remove(const IPollable & pollable);

		int Wait(int timeout);

		int Wait() //infinite wait
		{ return Wait(-1); }
	};
	TOOLKIT_DECLARE_PTR(Poll);

}}


#endif
