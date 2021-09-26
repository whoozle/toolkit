#ifndef TOOLKIT_IO_BUFFEREDSTREAM_H
#define TOOLKIT_IO_BUFFEREDSTREAM_H

#include <toolkit/io/IStream.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/ByteArray.h>

namespace TOOLKIT_NS { namespace io
{
	class BufferedStreamBase : public virtual IFlushable
	{
	protected:
		ByteArray 	_buffer;
		size_t		_offset;

	public:
		BufferedStreamBase(size_t bufferSize): _buffer(bufferSize), _offset(0)
		{ }
	};

	class BufferedOutputStream :
		public virtual BufferedStreamBase,
		public virtual IOutputStream
	{
	protected:
		IOutputStreamPtr	_stream;

	public:
		BufferedOutputStream(IOutputStreamPtr stream, size_t bufferSize):
			BufferedStreamBase(bufferSize), _stream(stream)
		{ }

		size_t Write(ConstBuffer data) override
		{
			auto srcData = data.GetPointer();
			auto remainingSrcData = data.GetSize();
			while(remainingSrcData != 0)
			{
				auto r = std::min(remainingSrcData, _buffer.size() - _offset);
				std::copy(srcData, srcData + r, _buffer.data() + _offset);
				srcData += r;
				_offset += r;
				remainingSrcData -= r;
				if (_offset == _buffer.size())
					Flush();
			}
			return data.GetSize();
		}

		void Flush() override
		{
			if (_offset)
			{
				if (_stream->Write(ConstBuffer(_buffer.data(), 0, _offset)) != _offset)
					throw Exception("short write");
				_offset = 0;
			}
		}

		~BufferedOutputStream()
		{ BufferedOutputStream::Flush(); }
	};

	class BufferedSeekableStream :
		public virtual ISeekable
	{
		ISeekablePtr _stream;

	public:
		BufferedSeekableStream(const ISeekablePtr & stream):
			_stream(stream)
		{ }

		off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) override
		{
			auto flushable = std::dynamic_pointer_cast<IFlushable>(_stream);
			if (flushable)
				flushable->Flush();
			//FIXME: relative seek does not work
			return _stream->Seek(offset, mode);
		}

		off_t Tell() override
		{ return _stream->Tell(); }
	};

	class BufferedInputStream :
		public virtual BufferedStreamBase,
		public virtual IInputStream
	{
	protected:
		IInputStreamPtr		_stream;
		size_t				_bufferSize;

	public:
		BufferedInputStream(IInputStreamPtr stream, size_t bufferSize):
			BufferedStreamBase(bufferSize), _stream(stream)
		{ _bufferSize = 0; }

		size_t Read(o1::Buffer data) override
		{
			auto dstData = data.GetPointer();
			size_t dstPos = 0;
			while(dstPos != data.size())
			{
				if (_offset == _bufferSize)
				{
					_bufferSize = _stream->Read(_buffer);
					_offset = 0;
				}

				auto r = std::min(data.size() - dstPos, _bufferSize - _offset);
				auto srcData = _buffer.data() + _offset;
				std::copy(srcData, srcData + r, dstData);
				dstData += r;
				dstPos += r;
				_offset += r;
			}
			return data.GetSize();
		}

		void Flush() override
		{
			_offset = 0;
			_bufferSize = 0;
		}
	};

	class BufferedStream:
		public BufferedInputStream,
		public BufferedOutputStream,
		public BufferedSeekableStream,
		public virtual IStorage
	{
		IStoragePtr _storage;

	public:
		BufferedStream(const IStoragePtr & storage, size_t bufferSize):
			BufferedStreamBase(bufferSize),
			BufferedInputStream(std::static_pointer_cast<IInputStream>(storage), bufferSize),
			BufferedOutputStream(std::static_pointer_cast<IOutputStream>(storage), bufferSize),
			BufferedSeekableStream(std::static_pointer_cast<ISeekable>(storage)),
			_storage(storage)
		{ }

		void Sync(io::SyncMode mode) override
		{ _storage->Sync(mode); }

		void Truncate(size_t size) override
		{ _storage->Truncate(size); }

		void Flush() override
		{ BufferedOutputStream::Flush(); }
	};

}}


#endif
