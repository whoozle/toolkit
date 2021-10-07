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

	template<typename StreamType>
	class BufferedSeekableOutputStreamBase : public BufferedOutputStreamBase<StreamType>
	{
		using super = BufferedOutputStreamBase<StreamType>;

	public:
		using super::super;

		off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) override
		{
			bool sameOffset = (mode == SeekMode::Begin && offset == Tell()) ||
				(mode == SeekMode::Current && offset == 0);
			if (sameOffset)
				return Tell();
			return this->_stream->Seek(offset, mode);
		}

		off_t Tell() override
		{ return this->_stream->Tell() + this->_offset; }
	};
	using BufferedSeekableOutputStream = BufferedSeekableOutputStreamBase<ISeekableOutputStream>;
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
					if (_bufferSize == 0)
						throw Exception("zero sized read");
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

	template<typename StreamType>
	class BufferedSeekableInputStreamBase : public BufferedInputStreamBase<StreamType>
	{
		using super = BufferedInputStreamBase<StreamType>;

	public:
		using super::super;

		off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) override
		{
			auto streamOffset = Tell();
			bool sameOffset = (mode == SeekMode::Begin && offset == streamOffset) ||
				(mode == SeekMode::Current && offset == 0);
			if (sameOffset)
				return streamOffset;

			off_t delta;
			off_t dstOffset;
			switch (mode)
			{
				case SeekMode::Begin:
					delta = offset - streamOffset;
					dstOffset = offset;
					break;
				case SeekMode::Current:
					delta = offset;
					dstOffset = streamOffset + offset;
					break;
				case SeekMode::End:
					this->Flush();
					return this->_stream->Seek(offset, mode);
				default:
					throw Exception("invalid seek mode");
			}

			if (delta < 0 || this->_offset + delta >= this->_bufferSize)
			{
				this->_stream->Seek(dstOffset);
				this->Flush();
			}
			else
				this->_offset += delta;
			return dstOffset;
		}

		off_t Tell() override
		{ return this->_stream->Tell() + this->_offset - this->_bufferSize; }
	};
	using BufferedSeekableInputStream = BufferedSeekableInputStreamBase<ISeekableInputStream>;
	TOOLKIT_DECLARE_PTR(BufferedSeekableInputStream);

}}


#endif
