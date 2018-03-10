#include <toolkit/io/Poll.h>
#include <toolkit/io/SystemException.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <unistd.h>

TOOLKIT_NS_BEGIN
namespace io
{
	namespace
	{

#define M(EV, NEV) if ((EV & events) == EV) r |= NEV

		epoll_event Map(const IPollable & pollable, int events)
		{
			epoll_event ev = { };
			int r = 0;
			M(POLLIN,	Poll::EventInput);
			M(POLLOUT,	Poll::EventInput);
			M(POLLHUP,	Poll::EventHangup);
			M(POLLERR,	Poll::EventError);
			M(POLLPRI,	Poll::EventPriority);
			M(EPOLLET,	Poll::EventEdgeTriggered);
			ev.events = r;
			ev.data.ptr = const_cast<void *>(static_cast<const void *>(&pollable));
			return ev;
		}
	}


	Poll::Poll(): _fd(epoll_create1(0))
	{
		if (_fd == -1)
			throw io::SystemException("epoll_create1(0)");
	}

	Poll::~Poll()
	{
		close(_fd);
	}

	void Poll::Add(const IPollable & pollable, int events)
	{
		auto ev = Map(pollable, events);
		SYSTEM_CALL(epoll_ctl(_fd, EPOLL_CTL_ADD, pollable.GetFileDescriptor(), &ev));
	}

	void Poll::Modify(const IPollable & pollable, int events)
	{
		auto ev = Map(pollable, events);
		SYSTEM_CALL(epoll_ctl(_fd, EPOLL_CTL_MOD, pollable.GetFileDescriptor(), &ev));
	}

	void Poll::Remove(const IPollable & pollable, int events)
	{
		auto ev = Map(pollable, events);
		SYSTEM_CALL(epoll_ctl(_fd, EPOLL_CTL_DEL, pollable.GetFileDescriptor(), &ev));
	}

	size_t Poll::Wait(Event *events, size_t eventsMax, int timeout)
	{
		epoll_event pollEvents[eventsMax]; //fixme
		int r = epoll_wait(_fd, pollEvents, eventsMax, timeout);
		if (r == -1)
			throw io::SystemException("epoll_wait");

		epoll_event *src = pollEvents;
		Event *dst = events;

		for(int i = 0; i < r; ++i)
		{
			*dst++ = Event { static_cast<io::IPollable *>(src->data.ptr), src->events };
		}

		return r;
	}
}
TOOLKIT_NS_END
