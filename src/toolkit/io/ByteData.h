#ifndef TOOLKIT_BYTEDATA_H
#define TOOLKIT_BYTEDATA_H

#include <toolkit/core/Exception.h>
#include <toolkit/core/types.h>
#include <stddef.h>

TOOLKIT_NS_BEGIN
namespace io
{

	template<typename T>
	class PointerSizeHolder
	{
	private:
		T *		_ptr;
		size_t	_size;

	public:
		PointerSizeHolder(): _ptr(), _size(0)
		{ }

		template<typename U>
		PointerSizeHolder(PointerSizeHolder<U> o): _ptr(o.data()), _size(o.size())
		{ }

		template<typename U>
		PointerSizeHolder(PointerSizeHolder<U> o, size_t offset)
		{
			size_t size = o.size();
			if (offset > size)
				throw Exception("offset is bigger than size");
			_ptr = o.data() + offset;
			_size = size - offset;
		}

		template<typename U>
		PointerSizeHolder(PointerSizeHolder<U> o, size_t offset, size_t size): PointerSizeHolder<U>(o, offset)
		{
			if (size > _size)
				throw Exception("requested size is bigger than actual size");
			_size = size;
		}

		template<typename Storage>
		PointerSizeHolder(Storage & storage): _ptr(storage.data()), _size(storage.size())
		{ }

		template<typename Storage>
		PointerSizeHolder(Storage & storage, size_t size): _ptr(storage.data()), _size(size)
		{
			if (size > storage.size())
				throw Exception("invalid size");
		}

		template<typename U>
		PointerSizeHolder(U * ptr, size_t size): _ptr(ptr), _size(size)
		{ }

		T * GetPointer() const
		{ return _ptr; }

		T * data() const
		{ return _ptr; }

		size_t GetSize() const
		{ return _size; }

		size_t size() const
		{ return _size; }
	};

	using ByteData = PointerSizeHolder<u8>;
	using ConstByteData = PointerSizeHolder<const u8>;

}
TOOLKIT_NS_END

#endif
