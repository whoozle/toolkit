#ifndef TOOLKIT_BYTEDATASTREAM_H
#define TOOLKIT_BYTEDATASTREAM_H

#include <toolkit/core/Buffer.h>
#include <toolkit/io/IStream.h>
#include <toolkit/io/IBufferedStream.h>
#include <string.h>

namespace TOOLKIT_NS { namespace io
{
	class BufferStreamBase : virtual public ISeekable
	{
	protected:
		off_t _offset;

		virtual off_t GetBufferSize() const = 0;

	public:
		BufferStreamBase(): _offset(0)
		{ }

		off_t Seek(off_t offset, SeekMode mode = SeekMode::Begin) override
		{
			switch (mode)
			{
			case SeekMode::Begin:
				_offset = offset;
				break;
			case SeekMode::Current:
				_offset += offset;
				break;
			case SeekMode::End:
				_offset = GetBufferSize() + offset;
				break;
			}
			return _offset;
		}

		off_t Tell() override
		{ return _offset; }
	};

	class BufferInputStream :
		public BufferStreamBase,
		public virtual ISeekableInputStream,
		public virtual IBufferedInputStream
	{
		ConstBuffer		_src;

	private:
		off_t GetBufferSize() const override
		{ return _src.size(); }

	public:
		BufferInputStream(ConstBuffer src): _src(src) { }

		size_t Read(Buffer dst) override
		{
			size_t size = _src.size();
			size_t n = std::min(dst.size(), size - _offset);
			memcpy(dst.data(), _src.data() + _offset, n);
			_offset += n;
			return n;
		}

        ConstBuffer GetReadBuffer() const override
		{ return ConstBuffer(_src, _offset); }

        void ReadComplete(size_t bytes) override
		{
			ASSERT(bytes + _offset <= _src.size(), Exception, "ReadComplete: input buffer overflow");
			_offset += bytes;
		}
	};

	class BufferOutputStream :
		public BufferStreamBase,
		public virtual ISeekableOutputStream,
		public virtual IBufferedOutputStream
	{
	private:
		Buffer			_dst;

		off_t GetBufferSize() const override
		{ return _dst.size(); }

	public:
		BufferOutputStream(Buffer dst): _dst(dst) { }

		size_t Write(ConstBuffer src) override
		{
			Buffer dst(_dst, _offset);
			size_t n = std::min(dst.size(), src.size());
			_offset += n;
			memcpy(dst.data(), src.data(), n);
			return n;
		}

        Buffer GetWriteBuffer() const override
		{ return _dst; }

        void WriteComplete(size_t bytes) override
		{
			ASSERT(bytes + _offset <= _dst.size(), Exception, "WriteComplete: input buffer overflow");
			_offset += bytes;
		}

        void WriteSkip(size_t bytes) override
		{ WriteComplete(bytes); }
	};

}}


#endif
