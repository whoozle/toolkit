#ifndef TOOLKIT_BYTEDATA_H
#define TOOLKIT_BYTEDATA_H

#include <toolkit/core/Exception.h>
#include <toolkit/core/types.h>
#include <stddef.h>

TOOLKIT_SUBNS_BEGIN(io)

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
		PointerSizeHolder(PointerSizeHolder<U> & o): _ptr(o.GetPointer()), _size(o.GetSize())
		{ }

		template<typename U>
		PointerSizeHolder(PointerSizeHolder<U> & o, size_t size): _ptr(o.GetPointer()), _size(size)
		{
			if (size > o.GetSize())
				throw Exception("invalid size");
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
		size_t GetSize() const
		{ return _size; }
	};

	using ByteData = PointerSizeHolder<u8>;
	using ConstByteData = PointerSizeHolder<const u8>;

TOOLKIT_SUBNS_END

#endif
