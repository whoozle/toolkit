#ifndef TOOLKIT_MEMORYSTREAM_H
#define TOOLKIT_MEMORYSTREAM_H

#include <toolkit/io/ByteArray.h>
#include <toolkit/io/IStream.h>
#include <string.h>
#include <vector>

TOOLKIT_NS_BEGIN
namespace io
{

	class MemoryOutputStream : public IOutputStream
	{
		using Storage = std::vector<u8>;
		Storage _data;

	public:
		MemoryOutputStream(size_t initialCapacity = 4096): _data() { _data.reserve(initialCapacity); }

		size_t Write(ConstByteData src) override
		{
			size_t offset = _data.size();
			size_t size = src.size();
			_data.resize(offset + size);
			memcpy(_data.data() + offset, src.data(), size);
			return size;
		}

		Storage && GetData()
		{ return std::move(_data); }
	};

}
TOOLKIT_NS_END

#endif
