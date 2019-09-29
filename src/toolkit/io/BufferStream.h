#ifndef TOOLKIT_BYTEDATASTREAM_H
#define TOOLKIT_BYTEDATASTREAM_H

#include <toolkit/core/Buffer.h>
#include <toolkit/io/IStream.h>
#include <string.h>

namespace TOOLKIT_NS { namespace io
{

	class BufferInputStream : public virtual IInputStream
	{
		ConstBuffer		_src;
		size_t			_offset;

	public:
		BufferInputStream(ConstBuffer src): _src(src), _offset(0) { }

		size_t Read(Buffer dst) override
		{
			size_t size = _src.size();
			size_t n = std::min(dst.size(), size - _offset);
			memcpy(dst.data(), _src.data() + _offset, n);
			_offset += n;
			return n;
		}
	};

	class BufferOutputStream : public virtual IOutputStream
	{
		Buffer			_dst;
		size_t			_offset;

	public:
		BufferOutputStream(Buffer dst): _dst(dst), _offset(0) { }

		size_t Write(ConstBuffer src) override
		{
			Buffer dst(_dst, _offset);
			size_t n = std::min(dst.size(), src.size());
			_offset += n;
			memcpy(dst.data(), src.data(), n);
			return n;
		}
	};
	class BufferBidirectionalStream :
		public BufferInputStream,
		public BufferOutputStream,
		public virtual IBidirectionalStream
	{
	public:
		BufferBidirectionalStream(Buffer buffer):
			BufferInputStream(buffer),
			BufferOutputStream(buffer)
		{ }
	};

}}


#endif
