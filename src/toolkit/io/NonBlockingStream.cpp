#include <toolkit/io/NonBlockingStream.h>
#include <toolkit/io/Poll.h>
#include <limits.h>

namespace TOOLKIT_NS { namespace io
{
	namespace
	{
		static constexpr auto DefaultEvent = io::Poll::EventInput | io::Poll::EventError | io::Poll::EventHangup;
	}

	void NonBlockingStream::HandleSocketEvent(int event)
	{
		std::lock_guard<decltype(_lock)> l(_lock);
		if (event & (io::Poll::EventError | io::Poll::EventHangup))
		{
			_failed = true;
			_poll.Remove(_pollable);
			_recvStream.Write(ConstBuffer());
			return;
		}

		if (event & io::Poll::EventInput)
		{
			auto read = _pollableStream.Read(_readBuffer);
			for(size_t offset = 0; offset < read; )
			{
				offset += _recvStream.Write(ConstBuffer(_readBuffer, offset));
			}
		}

		if (event & io::Poll::EventOutput)
		{
			auto toWrite = std::min<size_t>(_writeQueue.size(), PIPE_BUF);
			if (toWrite != 0)
			{
				auto written = _pollableStream.Write(ConstBuffer(_writeQueue, 0, toWrite));
				_writeQueue.Pop(written);
			}
			if (_writeQueue.empty())
				_poll.Modify(_pollable, *this, DefaultEvent);
		}
	}

	NonBlockingStream::NonBlockingStream(Poll & poll, IPollable & pollable, IBidirectionalStream & pollableStream, IOutputStream & recvStream):
		_poll(poll), _pollable(pollable), _pollableStream(pollableStream), _recvStream(recvStream),
		_readBuffer(PIPE_BUF), _failed(false)
	{ _poll.Add(_pollable, *this, DefaultEvent); }

	NonBlockingStream::~NonBlockingStream()
	{ _poll.Remove(_pollable); }

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
