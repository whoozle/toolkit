#include <toolkit/io/NonBlockingStream.h>
#include <toolkit/io/Poll.h>
#include <toolkit/log/Logger.h>
#include <limits.h>

namespace TOOLKIT_NS { namespace io
{
	namespace
	{
		static constexpr auto DefaultEvent = io::Poll::EventInput | io::Poll::EventError | io::Poll::EventHangup;
		log::Logger Log("non-blocking-stream");
	}

	void NonBlockingStream::HandleSocketEvent(int event)
	{
		std::lock_guard<decltype(_lock)> l(_lock);
		if (event & (io::Poll::EventError | io::Poll::EventHangup))
		{
			_failed = true;
			if (event & io::Poll::EventHangup)
				_handler.OnError(PollError::Hangup);
			else
				_handler.OnError(PollError::Error);
			return;
		}

		if (event & io::Poll::EventInput)
		{ _handler.CanRead(); }

		if (event & io::Poll::EventOutput)
		{
			auto toWrite = std::min<size_t>(_writeQueue.size(), PIPE_BUF);
			if (toWrite != 0)
			{
				auto written = _handler.CanWrite(ConstBuffer(_writeQueue, 0, toWrite));
				_writeQueue.Pop(written);
			}
			if (_writeQueue.empty())
				_poll.Modify(_pollable, *this, DefaultEvent);
		}
	}

	NonBlockingStream::NonBlockingStream(Poll & poll, IPollable & pollable, INonBlockingStreamEventHandler & handler):
		_poll(poll), _pollable(pollable),
		_handler(handler),
		_failed(false)
	{ _poll.Add(_pollable, *this, DefaultEvent); }

	NonBlockingStream::~NonBlockingStream()
	{
		try { _poll.Remove(_pollable); }
		catch(const std::exception & ex)
		{ Log.Error() << "removing pollable failed: " << ex; }
	}

	size_t NonBlockingStream::Write(ConstBuffer data)
	{
		std::lock_guard<decltype(_lock)> l(_lock);
		bool wasEmpty = _writeQueue.empty();
		_writeQueue.Append(data);
		if (wasEmpty)
			_poll.Modify(_pollable, *this, DefaultEvent | io::Poll::EventOutput);
		return data.size();
	}

	bool NonBlockingStream::Failed() const
	{
		std::lock_guard<decltype(_lock)> l(_lock);
		return _failed;
	}

}}
