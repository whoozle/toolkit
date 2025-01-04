#include <toolkit/io/Notify.h>
#include <toolkit/io/Poll.h>
#include <toolkit/io/INotifyEventListener.h>
#include <toolkit/io/SystemException.h>
#include <toolkit/log/Logger.h>
#include <sys/inotify.h>

namespace TOOLKIT_NS { namespace io
{
	namespace
	{
		log::Logger Log("inotify");
	}

	void NotifyPollEventHandler::HandleSocketEvent(int event)
	{
		if (event & Poll::EventInput)
			_parent.Read(_listener);
	}

	class Notify::WatchHandler final : public INotifyWatchHandler
	{
		int _nd;
		int _wd;
	public:
		WatchHandler(int nd, int wd): _nd(nd), _wd(wd)
		{ }

		~WatchHandler()
		{
			TRY
			{ SYSTEM_CALL(inotify_rm_watch(_nd, _wd)); }
			CATCH(const Exception &ex)
			{
				Log.Error() << "inotify_rm_watch: " << ex;
			}
		}

		bool Matches(const NotifyEvent &ev) const override
		{ return ev.WatchDescriptor == _wd; }
	};

	int Notify::createINotify(int flags)
	{ SYSTEM_CALL_RETURN(inotify_init1(flags)); }

	Notify::Notify(int flags, size_t maxEvents): _fd(createINotify(flags)),
		_buffer(maxEvents * (sizeof(struct inotify_event) + NAME_MAX + 1))
	{ }

	Notify::~Notify() = default;

	int Notify::Add(int fd, const char *path, u32 mask)
	{ SYSTEM_CALL_RETURN(inotify_add_watch(fd, path, mask)); }

	int Notify::Add(const std::string &path, u32 mask)
	{
		int fd = _fd.GetFileDescriptor();
		return Add(fd, path.c_str(), mask);
	}

	INotifyWatchHandlerUPtr Notify::CreateWatchHandler(int wd)
	{ return std::make_unique<Notify::WatchHandler>(_fd.GetFileDescriptor(), wd); }

	void Notify::Read(INotifyEventListener & handler)
	{
		static constexpr size_t EventSize = sizeof(inotify_event);
		auto r = _fd.Read(_buffer);
		NotifyEvent ev;
		for(size_t p = 0; p < r; )
		{
			ConstBuffer buf(_buffer, p, r - p);
			ASSERT(buf.size() >= EventSize, Exception, "invalid size returned from read");
			auto *event = reinterpret_cast<const inotify_event *>(_buffer.data());
			ev.WatchDescriptor = event->wd;
			ev.Cookie = event->cookie;
			ev.Mask = event->mask;
			if (event->len)
				ev.Name = {event->name, event->len - 1};
			else
				ev.Name = {};
			handler.HandleNotifyEvent(ev);
			p += EventSize;
			p += event->len;
		}
	}
}}
