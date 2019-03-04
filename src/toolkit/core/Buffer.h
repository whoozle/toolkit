#ifndef TOOLKIT_BYTEDATA_H
#define TOOLKIT_BYTEDATA_H

#include <toolkit/core/Exception.h>
#include <toolkit/core/types.h>
#include <stddef.h>

TOOLKIT_NS_BEGIN

	template<typename T>
	class BasicBuffer
	{
	private:
		T *		_ptr;
		size_t	_size;

	public:
		BasicBuffer(): _ptr(), _size(0)
		{ }

		template <typename U, size_t size>
		BasicBuffer(U (&array)[size]): _ptr(array), _size(size)
		{ }

		template<typename U>
		BasicBuffer(BasicBuffer<U> o): _ptr(o.data()), _size(o.size())
		{ }

		template<typename U>
		BasicBuffer(BasicBuffer<U> o, size_t offset)
		{
			size_t size = o.size();
			if (offset > size)
				throw Exception("offset is bigger than size");
			_ptr = o.data() + offset;
			_size = size - offset;
		}

		template<typename U>
		BasicBuffer(BasicBuffer<U> o, size_t offset, size_t size): BasicBuffer<U>(o, offset)
		{
			if (size > _size)
				throw Exception("requested size is bigger than actual size");
			_size = size;
		}

		template<typename Storage>
		BasicBuffer(Storage & storage): _ptr(storage.data()), _size(storage.size())
		{ }

		template<typename Storage>
		BasicBuffer(Storage & storage, size_t size): _ptr(storage.data()), _size(size)
		{
			if (size > storage.size())
				throw Exception("invalid size");
		}

		T & operator[] (size_t index)
		{
			if (index >= _size)
				throw Exception("invalid index for operator[]");
			return _ptr[index];
		}

		T operator[] (size_t index) const
		{
			if (index >= _size)
				throw Exception("invalid index for operator[]");
			return _ptr[index];
		}

		template<typename U>
		BasicBuffer(U * ptr, size_t size): _ptr(ptr), _size(size)
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

	using Buffer		= BasicBuffer<u8>;
	using ConstBuffer	= BasicBuffer<const u8>;

TOOLKIT_NS_END

#endif
