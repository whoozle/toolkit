#ifndef TOOLKIT_IO_PEEKABLESTREAM_H
#define TOOLKIT_IO_PEEKABLESTREAM_H

#include <toolkit/core/core.h>
#include <toolkit/core/Buffer.h>
#include <toolkit/io/IStream.h>
#include <memory>
#include <string.h>

namespace TOOLKIT_NS { namespace io
{

	struct IBufferedInputStream : public virtual IInputStream
	{
		virtual ~IBufferedInputStream() = default;

        virtual ConstBuffer GetReadBuffer() const = 0;
        virtual void ReadComplete(size_t bytes) = 0;

		size_t Read(Buffer data) override
		{
			auto dst = data.data();
			auto size = data.size();

			size_t offset = 0;
			while(offset < size)
			{
				auto src = GetReadBuffer();
				auto tailSize = size - offset;
				auto readSize = std::min(tailSize, src.size());
				memcpy(dst, src.data(), readSize);
				dst += readSize;
				offset += readSize;
			}
			return size;
		}
	};
	TOOLKIT_DECLARE_PTR(IBufferedInputStream);

	struct IBufferedOutputStream : public virtual IOutputStream
	{
		virtual ~IBufferedOutputStream() = default;

        virtual Buffer GetWriteBuffer() const = 0;
        virtual void WriteComplete(size_t bytes) = 0;
        virtual void WriteSkip(size_t bytes) = 0;

		size_t Write(ConstBuffer data) override
		{
			auto src = data.data();
			auto size = data.size();

			size_t offset = 0;
			while(offset < size)
			{
				auto buffer = GetWriteBuffer();
				auto tailSize = size - offset;
				auto writeSize = std::min(buffer.size(), tailSize);
				memcpy(buffer.data(), src, writeSize);
				src += writeSize;
				offset += writeSize;
				WriteComplete(writeSize);
			}
			return size;
		}
	};
	TOOLKIT_DECLARE_PTR(IBufferedOutputStream);

	struct IBufferedBidirectionalStream :
        virtual IBufferedInputStream,
        virtual IBufferedOutputStream
	{ };
	TOOLKIT_DECLARE_PTR(IBufferedBidirectionalStream);

}}


#endif
