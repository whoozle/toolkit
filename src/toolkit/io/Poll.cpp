#include <toolkit/io/Poll.h>
#include <toolkit/io/SystemException.h>
#include <toolkit/io/IPollable.h>
#include <toolkit/io/IPollEventHandler.h>
#include <toolkit/log/Logger.h>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <unistd.h>

namespace TOOLKIT_NS { namespace io
{
	namespace
	{
		log::Logger Log("poll");

#define M(EV, NEV) if ((NEV & events) == NEV) r |= EV
#define U(EV, NEV) if ((EV & events) == EV) r |= NEV

		epoll_event Map(const IPollEventHandler & handler, int events)
		{
			epoll_event ev = { };
			u32 r = 0;
			M(POLLIN,	Poll::EventInput);
			M(POLLOUT,	Poll::EventOutput);
			M(POLLHUP,	Poll::EventHangup);
			M(POLLERR,	Poll::EventError);
			M(POLLPRI,	Poll::EventPriority);
			M(EPOLLET,	Poll::EventEdgeTriggered);
			ev.events = r;
			ev.data.ptr = const_cast<void *>(static_cast<const void *>(&handler));
			return ev;
		}

		u32 Unmap(int events)
		{
			u32 r = 0;
			U(POLLIN,	Poll::EventInput);
			U(POLLOUT,	Poll::EventOutput);
			U(POLLHUP,	Poll::EventHangup);
			U(POLLERR,	Poll::EventError);
			U(POLLPRI,	Poll::EventPriority);
			U(EPOLLET,	Poll::EventEdgeTriggered);
			return r;
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

	void Poll::Add(const IPollable & pollable, IPollEventHandler & handler, int events)
	{
		auto ev = Map(handler, events);
		SYSTEM_CALL(epoll_ctl(_fd, EPOLL_CTL_ADD, pollable.GetFileDescriptor(), &ev));
	}

	void Poll::Modify(const IPollable & pollable, IPollEventHandler & handler, int events)
	{
		auto ev = Map(handler, events);
		SYSTEM_CALL(epoll_ctl(_fd, EPOLL_CTL_MOD, pollable.GetFileDescriptor(), &ev));
	}

	void Poll::Remove(const IPollable & pollable)
	{
		SYSTEM_CALL(epoll_ctl(_fd, EPOLL_CTL_DEL, pollable.GetFileDescriptor(), nullptr));
	}

	int Poll::Wait(int timeout)
	{
		epoll_event pollEvents[MaxEvents]; //fixme
		int r = epoll_wait(_fd, pollEvents, MaxEvents, timeout);
		if (r < 0) {
			if (errno == EINTR) //interrupted system call is normal
				return 0;
			else
				throw io::SystemException("epoll_wait");
		}

		epoll_event *src = pollEvents;

		for(int n = r; n--; ++src)
		{
			IPollEventHandler *handler = static_cast<IPollEventHandler *>(src->data.ptr);
			try { handler->HandleSocketEvent(Unmap(src->events)); }
			catch(const std::exception & ex)
			{ Log.Error() << "HandleSocketEvent failed: " << ex.what(); }
		}
		return r;
	}
}}

