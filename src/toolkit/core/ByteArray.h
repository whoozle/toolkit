#ifndef TOOLKIT_IO_BYTEARRAY_H
#define TOOLKIT_IO_BYTEARRAY_H

#include <toolkit/core/Buffer.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/types.h>
#include <vector>
#include <memory>

namespace TOOLKIT_NS
{

	class ByteArray : public Noncopyable
	{
		std::vector<u8>		_data;

	public:
		using const_iterator	= std::vector<u8>::const_iterator;
		using iterator 			= std::vector<u8>::iterator;

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
		void clear()
		{ _data.clear(); }

		void Resize(size_t size)
		{ return _data.resize(size); }
		void Reserve(size_t size)
		{ _data.reserve(size); }
		void Clear()
		{ _data.clear(); }

		iterator begin() 				{ return _data.begin(); }
		iterator end() 					{ return _data.begin(); }
		const_iterator begin() const	{ return _data.begin(); }
		const_iterator end() const		{ return _data.begin(); }
	};
	DECLARE_PTR(ByteArray);

}

#endif
