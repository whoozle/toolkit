#ifndef TOOLKIT_IO_NOTIFY_H
#define TOOLKIT_IO_NOTIFY_H

#include <toolkit/core/ByteArray.h>
#include <toolkit/io/File.h>
#include <toolkit/io/IPollEventHandler.h>
#include <string>

namespace TOOLKIT_NS { namespace io
{
	struct INotifyEventListener;
	struct INotifyWatchHandler;
	TOOLKIT_DECLARE_UPTR(INotifyWatchHandler);

	class Notify;
	class NotifyPollEventHandler final: public IPollEventHandler
	{
		Notify & 				_parent;
		INotifyEventListener &	_listener;

	public:
		NotifyPollEventHandler(Notify &parent, INotifyEventListener &listener):
			_parent(parent), _listener(listener)
		{ }

	private:
		void HandleSocketEvent(int event);
	};

	class Notify final: public IPollable
	{
	private:
		io::File 	_fd;
		ByteArray 	_buffer;

		class WatchHandler;

		static int createINotify(int flags);

	public:
		Notify(int flags = 0, size_t maxEvents = 128);
		~Notify();

		int GetFileDescriptor() const override
		{ return _fd.GetFileDescriptor(); }

		static int Add(int fd, const char *path, u32 mask);
		int Add(const std::string &path, u32 mask);

		INotifyWatchHandlerUPtr CreateWatchHandler(int wd);

		void Read(INotifyEventListener & handler);
	};

}}

#endif
