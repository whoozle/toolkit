#include <toolkit/io/Poll.h>
#include <toolkit/io/SystemException.h>
#include <toolkit/io/IPollable.h>
#include <toolkit/io/ISocketEventHandler.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <unistd.h>

TOOLKIT_NS_BEGIN
namespace io
{
	namespace
	{

#define M(EV, NEV) if ((EV & events) == EV) r |= NEV

		epoll_event Map(const ISocketEventHandler & handler, int events)
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
			ev.data.ptr = const_cast<void *>(static_cast<const void *>(&handler));
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

	void Poll::Add(const IPollable & pollable, ISocketEventHandler & handler, int events)
	{
		auto ev = Map(handler, events);
		SYSTEM_CALL(epoll_ctl(_fd, EPOLL_CTL_ADD, pollable.GetFileDescriptor(), &ev));
	}

	void Poll::Modify(const IPollable & pollable, ISocketEventHandler & handler, int events)
	{
		auto ev = Map(handler, events);
		SYSTEM_CALL(epoll_ctl(_fd, EPOLL_CTL_MOD, pollable.GetFileDescriptor(), &ev));
	}

	void Poll::Remove(const IPollable & pollable, ISocketEventHandler & handler, int events)
	{
		auto ev = Map(handler, events);
		SYSTEM_CALL(epoll_ctl(_fd, EPOLL_CTL_DEL, pollable.GetFileDescriptor(), &ev));
	}

	void Poll::Wait(int timeout)
	{
		epoll_event pollEvents[MaxEvents]; //fixme
		int r = epoll_wait(_fd, pollEvents, MaxEvents, timeout);
		if (r < 0)
			throw io::SystemException("epoll_wait");

		epoll_event *src = pollEvents;
		while(r--)
		{
			ISocketEventHandler *handler = static_cast<ISocketEventHandler *>(src->data.ptr);
			handler->HandleSocketEvent(src->events);
		}
	}
}
TOOLKIT_NS_END
