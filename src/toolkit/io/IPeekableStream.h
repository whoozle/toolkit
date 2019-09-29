#ifndef TOOLKIT_IO_PEEKABLESTREAM_H
#define TOOLKIT_IO_PEEKABLESTREAM_H

#include <toolkit/core/core.h>
#include <toolkit/core/Buffer.h>
#include <memory>

namespace TOOLKIT_NS { namespace io
{

	struct IPeekableInputStream
	{
		virtual ~IPeekableInputStream() = default;

        virtual ConstBuffer GetReadBuffer() const = 0;
        virtual void ReadComplete(size_t bytes) = 0;
	};
	TOOLKIT_DECLARE_PTR(IPeekableInputStream);

	struct IPeekableOutputStream
	{
		virtual ~IPeekableOutputStream() = default;

        virtual Buffer GetWriteBuffer() const = 0;
        virtual void WriteComplete(size_t bytes) = 0;
        virtual void WriteSkip(size_t bytes) = 0;
	};
	TOOLKIT_DECLARE_PTR(IPeekableOutputStream);

	struct IPeekableBidirectionalStream :
        virtual IPeekableInputStream,
        virtual IPeekableOutputStream
	{ };
	TOOLKIT_DECLARE_PTR(IPeekableBidirectionalStream);

}}


#endif
