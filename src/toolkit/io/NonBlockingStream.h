#ifndef TOOLKIT_IO_NONBLOCKINGSTREAM_H
#define TOOLKIT_IO_NONBLOCKINGSTREAM_H

#include <toolkit/core/Buffer.h>
#include <toolkit/core/ByteArray.h>
#include <toolkit/io/IPollEventHandler.h>
#include <toolkit/io/IStream.h>
#include <mutex>

namespace TOOLKIT_NS { namespace io
{
	class Poll;
	struct IPollable;

	class NonBlockingStream : public io::IOutputStream, private IPollEventHandler
	{
	public:

	private:
		mutable std::recursive_mutex	_lock;
		Poll & 							_poll;
		IPollable & 					_pollable;
		IBidirectionalStream & 			_pollableStream;
		IOutputStream &					_recvStream;
		ByteArray						_writeQueue;
		ByteArray						_readBuffer;
		bool							_failed;


	public:
		NonBlockingStream(Poll & poll, IPollable & pollable, IBidirectionalStream & pollableStream, IOutputStream & recvStream);
		~NonBlockingStream();

		size_t Write(ConstBuffer data) override;
		bool Failed() const;

	private:
		void HandleSocketEvent(int event) override;
	};

}}

#endif
