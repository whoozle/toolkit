#ifndef TOOLKIT_IO_BYTEARRAY_H
#define TOOLKIT_IO_BYTEARRAY_H

#include <toolkit/io/ByteData.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/types.h>
#include <vector>
#include <memory>

TOOLKIT_NS_BEGIN
namespace io
{

	class ByteArray : public Noncopyable
	{
		std::vector<u8>		_data;

	public:
		ByteArray(size_t size): _data(size) { }
		ByteArray(std::vector<u8> && data): _data(data) { }
		ByteArray(ByteArray && data): ByteArray(std::move(data._data)) { }

		u8 * GetData()
		{ return _data.data(); }
		const u8 * GetData() const
		{ return _data.data(); }
		size_t GetSize() const
		{ return _data.size(); }

		u8 * data()
		{ return _data.data(); }
		const u8 * data() const
		{ return _data.data(); }
		size_t size() const
		{ return _data.size(); }

		void Resize(size_t size)
		{ return _data.resize(size); }
		void Reserve(size_t size)
		{ _data.reserve(size); }
	};
	DECLARE_PTR(ByteArray);

}
TOOLKIT_NS_END

#endif
