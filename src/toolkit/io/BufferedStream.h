#ifndef TOOLKIT_IO_BUFFEREDSTREAM_H
#define TOOLKIT_IO_BUFFEREDSTREAM_H

#include <toolkit/io/IStream.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/ByteArray.h>

namespace TOOLKIT_NS { namespace io
{
	class BufferedStreamBase
	{
	protected:
		ByteArray 	_buffer;
		size_t		_offset;

	public:
		BufferedStreamBase(size_t bufferSize): _buffer(bufferSize), _offset(0)
		{ }
	};

	template<typename StreamType>
	class BufferedOutputStreamBase :
		public BufferedStreamBase,
		public virtual IFlushable,
		public virtual StreamType
	{
	protected:
		using StreamPtr = std::shared_ptr<StreamType>;
		StreamPtr	_stream;

	public:
		BufferedOutputStreamBase(const StreamPtr & stream, size_t bufferSize):
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
				if (_stream->Write(ConstBuffer(_buffer, 0, _offset)) != _offset)
					throw Exception("short write");
				_offset = 0;
			}
		}

		~BufferedOutputStreamBase()
		{ BufferedOutputStreamBase::Flush(); }
	};

	using BufferedOutputStream = BufferedOutputStreamBase<IOutputStream>;
	TOOLKIT_DECLARE_PTR(BufferedOutputStream);

	class BufferedSeekableOutputStream : public BufferedOutputStreamBase<ISeekableOutputStream>
	{
		using super = BufferedOutputStreamBase<ISeekableOutputStream>;

	public:
		using super::super;

		off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) override
		{
			Flush();
			return _stream->Seek(offset, mode);
		}

		off_t Tell() override
		{ return _stream->Tell() + _offset; }
	};
	TOOLKIT_DECLARE_PTR(BufferedSeekableOutputStream);


	template<typename StreamType>
	class BufferedInputStreamBase :
		public BufferedStreamBase,
		public virtual IFlushable,
		public virtual StreamType
	{
	protected:
		using StreamPtr = std::shared_ptr<StreamType>;
		StreamPtr			_stream;
		size_t				_bufferSize;

	public:
		BufferedInputStreamBase(const StreamPtr & stream, size_t bufferSize):
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

	using BufferedInputStream = BufferedInputStreamBase<IInputStream>;
	TOOLKIT_DECLARE_PTR(BufferedInputStream);

	class BufferedSeekableInputStream : public BufferedInputStreamBase<ISeekableInputStream>
	{
		using super = BufferedInputStreamBase<ISeekableInputStream>;

	public:
		using super::super;

		off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) override
		{
			Flush();
			return _stream->Seek(offset, mode);
		}

		off_t Tell() override
		{ return _stream->Tell() + _offset; }
	};
	TOOLKIT_DECLARE_PTR(BufferedSeekableInputStream);

	class BufferedStream:
		public BufferedSeekableInputStream,
		public BufferedSeekableOutputStream
	{
	public:
		BufferedStream(const IStoragePtr & stream, size_t bufferSize):
			BufferedSeekableInputStream(std::static_pointer_cast<ISeekableInputStream>(stream), bufferSize),
			BufferedSeekableOutputStream(std::static_pointer_cast<ISeekableOutputStream>(stream), bufferSize)
		{ }

		off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) override
		{
			BufferedSeekableInputStream::Seek(offset, mode);
			return BufferedSeekableOutputStream::Seek(offset, mode);
		}

		off_t Tell() override
		{ return BufferedSeekableOutputStream::Tell(); }

		void Flush() override
		{
			BufferedSeekableOutputStream::Flush();
		}
	};

}}


#endif
