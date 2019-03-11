#ifndef TOOLKIT_IO_COPY_H
#define TOOLKIT_IO_COPY_H

#include <toolkit/io/IStream.h>
#include <toolkit/core/ByteArray.h>
#include <toolkit/core/Buffer.h>

TOOLKIT_NS_BEGIN
namespace io
{

	inline void Copy(IStorage & dst, IStorage & src, size_t bufferSize = 128 * 1024)
	{
		ByteArray buffer(bufferSize);
		size_t r;
		do
		{
			r = src.Read(buffer);
			dst.Write(ConstBuffer(buffer, r));
		}
		while(r == bufferSize);
	}

}
TOOLKIT_NS_END


#endif

