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

	class NonBlockingStream : public io::IOutputStream, private IPollEventHandler
	{
	public:
		using InputCallback = std::function<void()>;
		using OutputCallback = std::function<size_t (ConstBuffer)>;
		using ErrorCallback = std::function<void()>;

	private:
		mutable std::recursive_mutex	_lock;
		Poll & 							_poll;
		IPollable & 					_pollable;
		InputCallback					_inputCallback;
		OutputCallback					_outputCallback;
		ErrorCallback					_errorCallback;
		ByteArray						_writeQueue;
		bool							_failed;


	public:
		NonBlockingStream(Poll & poll, IPollable & pollable,
			InputCallback inputCallback, OutputCallback outputCallback, ErrorCallback errorCallback);
		~NonBlockingStream();

		size_t Write(ConstBuffer data) override;
		bool Failed() const;

	private:
		void HandleSocketEvent(int event) override;
	};

}}

#endif
