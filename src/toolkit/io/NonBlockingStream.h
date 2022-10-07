#ifndef TOOLKIT_IO_NONBLOCKINGSTREAM_H
#define TOOLKIT_IO_NONBLOCKINGSTREAM_H

#include <toolkit/core/Buffer.h>
#include <toolkit/core/ByteArray.h>
#include <toolkit/io/IPollEventHandler.h>
#include <toolkit/io/IStream.h>
#include <functional>
#include <mutex>

namespace TOOLKIT_NS { namespace io
{
	class Poll;
	struct IPollable;

	enum struct PollError
	{
		Error,
		Hangup
	};

	struct INonBlockingStreamEventHandler
	{
		virtual ~INonBlockingStreamEventHandler() = default;

		virtual void CanRead() = 0;
		virtual size_t CanWrite(ConstBuffer data) = 0;
		virtual void OnError(PollError error) = 0;
	};

	class NonBlockingStream : public io::IOutputStream, private IPollEventHandler
	{
	public:

	private:
		mutable std::recursive_mutex	_lock;
		Poll & 							_poll;
		IPollable & 					_pollable;
		INonBlockingStreamEventHandler &_handler;
		ByteArray						_writeQueue;
		bool							_failed;


	public:
		NonBlockingStream(Poll & poll, IPollable & pollable, INonBlockingStreamEventHandler & handler);
		~NonBlockingStream();

		size_t Write(ConstBuffer data) override;
		bool Failed() const;

	private:
		void HandleSocketEvent(int event) override;
	};

}}

#endif
