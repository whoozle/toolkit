#ifndef TOOLKIT_MEMORYOUTPUTSTREAM_H
#define TOOLKIT_MEMORYOUTPUTSTREAM_H

#include <toolkit/core/ByteArray.h>
#include <toolkit/io/IStream.h>
#include <string.h>
#include <vector>

namespace TOOLKIT_NS { namespace io
{

	class MemoryOutputStream : public IOutputStream
	{
		using Storage = std::vector<u8>;
		Storage _data;

	public:
		MemoryOutputStream(size_t initialCapacity = 4096): _data() { _data.reserve(initialCapacity); }

		size_t Write(ConstBuffer src) override
		{
			size_t offset = _data.size();
			size_t size = src.size();
			_data.resize(offset + size);
			memcpy(_data.data() + offset, src.data(), size);
			return size;
		}

		size_t GetSize() const
		{ return _data.size(); }

		Storage && GetData()
		{ return std::move(_data); }
	};

}}


#endif
