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
		using Storage = ByteArray;
		Storage _data;

	public:
		static constexpr size_t InitialCapacity = 4096;

		MemoryOutputStream(size_t initialCapacity = InitialCapacity): _data()
		{ _data.Reserve(initialCapacity); }

		size_t Write(ConstBuffer src) override
		{
			auto size = src.size();
			auto dst = Append(size);
			memcpy(dst.data(), src.data(), size);
			return size;
		}

		Buffer Append(size_t size)
		{
			size_t offset = _data.size();
			_data.Resize(offset + size);
			return Buffer(_data.data() + offset, size);
		}

		size_t GetSize() const
		{ return _data.size(); }

		const Storage & GetStorage() const
		{ return _data; }

		Storage & GetStorage()
		{ return _data; }

		ByteArray Extract()
		{ return std::move(_data); }
	};

}}


#endif
