#ifndef TOOLKIT_IO_PEEKABLESTREAM_H
#define TOOLKIT_IO_PEEKABLESTREAM_H

#include <toolkit/core/core.h>
#include <toolkit/core/Buffer.h>
#include <memory>

namespace TOOLKIT_NS { namespace io
{

	struct IBufferedInputStream
	{
		virtual ~IBufferedInputStream() = default;

        virtual ConstBuffer GetReadBuffer() const = 0;
        virtual void ReadComplete(size_t bytes) = 0;
	};
	TOOLKIT_DECLARE_PTR(IBufferedInputStream);

	struct IBufferedOutputStream
	{
		virtual ~IBufferedOutputStream() = default;

        virtual Buffer GetWriteBuffer() const = 0;
        virtual void WriteComplete(size_t bytes) = 0;
        virtual void WriteSkip(size_t bytes) = 0;
	};
	TOOLKIT_DECLARE_PTR(IBufferedOutputStream);

	struct IBufferedBidirectionalStream :
        virtual IBufferedInputStream,
        virtual IBufferedOutputStream
	{ };
	TOOLKIT_DECLARE_PTR(IBufferedBidirectionalStream);

}}


#endif
