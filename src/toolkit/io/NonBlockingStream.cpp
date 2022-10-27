#include <toolkit/io/NonBlockingStream.h>
#include <toolkit/io/Poll.h>
#include <limits.h>

namespace TOOLKIT_NS { namespace io
{
	namespace
	{
		static constexpr auto DefaultEvent = io::Poll::EventInput | io::Poll::EventError | io::Poll::EventHangup;
	}

	bool NonBlockingStream::WriteNext()
	{
		if (_writeQueue.empty())
			return false;

		auto & top = _writeQueue.front();
		auto toWrite = std::min<size_t>(top.size(), PIPE_BUF);
		if (toWrite != 0)
		{
			auto written = _handler.CanWrite(ConstBuffer(top, 0, toWrite));
			ASSERT(written <= toWrite, Exception, "returned more data written than was requested");
			if (written == toWrite)
				_writeQueue.pop();
			else
				top.Pop(written);
		}

		return !_writeQueue.empty();
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
			if (!WriteNext())
				_poll.Modify(_pollable, *this, DefaultEvent);
		}
	}

	void NonBlockingStream::Sync()
	{
		std::lock_guard<decltype(_lock)> l(_lock);
		while(WriteNext());
		_poll.Modify(_pollable, *this, DefaultEvent);
	}

	NonBlockingStream::NonBlockingStream(Poll & poll, IPollable & pollable, INonBlockingStreamEventHandler & handler):
		_poll(poll), _pollable(pollable),
		_handler(handler),
		_failed(false)
	{ _poll.Add(_pollable, *this, DefaultEvent); }

	NonBlockingStream::~NonBlockingStream()
	{ _poll.Remove(_pollable); }

	size_t NonBlockingStream::Write(ConstBuffer data)
	{
		std::lock_guard<decltype(_lock)> l(_lock);
		bool wasEmpty = _writeQueue.empty();
		_writeQueue.push(data);
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
