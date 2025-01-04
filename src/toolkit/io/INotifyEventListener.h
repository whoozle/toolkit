#ifndef INOTIFYEVENTLISTENER_H
#define INOTIFYEVENTLISTENER_H

#include <toolkit/core/types.h>
#include <memory>
#include <string_view>

namespace TOOLKIT_NS { namespace io
{
	struct NotifyEvent;

	struct INotifyWatchHandler
	{
		virtual ~INotifyWatchHandler() = default;
		virtual bool Matches(const NotifyEvent &ev) const = 0;
	};
	TOOLKIT_DECLARE_PTR(INotifyWatchHandler);
	TOOLKIT_DECLARE_UPTR(INotifyWatchHandler);

	struct NotifyEvent
	{
		int WatchDescriptor;
		u32 Mask;
		u32 Cookie;
		std::string_view Name;
	};

	struct INotifyEventListener
	{
		virtual ~INotifyEventListener() = default;
		virtual void HandleNotifyEvent(const NotifyEvent &event) = 0;
	};

}}



#endif
