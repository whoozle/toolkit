#ifndef TOOLKIT_BYTEDATA_H
#define TOOLKIT_BYTEDATA_H

#include <toolkit/core/Exception.h>
#include <toolkit/core/types.h>
#include <stddef.h>

namespace TOOLKIT_NS
{

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
		BasicBuffer(U && o): _ptr(o.data()), _size(o.size())
		{ }

		template<typename U>
		BasicBuffer(U && o, size_t offset)
		{
			size_t size = o.size();
			if (offset > size)
				throw Exception("offset is bigger than size");
			_ptr = o.data() + offset;
			_size = size - offset;
		}

		template<typename U>
		BasicBuffer(U && o, size_t offset, size_t size): BasicBuffer(o, offset)
		{
			if (size > _size)
				throw Exception("requested size (" + std::to_string(size) + ") is bigger than actual size (" + std::to_string(_size) + ")");
			_size = size;
		}

		BasicBuffer(T * ptr, size_t size): _ptr(ptr), _size(size)
		{ }

		BasicBuffer(T * ptr, size_t offset, size_t size): _ptr(ptr + offset)
		{
			if (offset > size)
				throw Exception("requested offset (" + std::to_string(offset) + ") is bigger than buffer size (" + std::to_string(_size) + ")");
			_size = size - offset;
		}

		T & operator[] (size_t index)
		{
			if (index >= _size)
				throw Exception("invalid index (" + std::to_string(index) + ") for operator[]");
			return _ptr[index];
		}

		T operator[] (size_t index) const
		{
			if (index >= _size)
				throw Exception("invalid index (" + std::to_string(index) + ") for operator[]");
			return _ptr[index];
		}

		template<typename U>
		BasicBuffer(U * ptr, size_t size): _ptr(ptr), _size(size)
		{ }

		T * GetPointer() const
		{ return _ptr; }

		T * data()
		{ return _ptr; }

		T * data() const
		{ return _ptr; }

		T * begin()
		{ return _ptr; }

		T * begin() const
		{ return _ptr; }

		T * end()
		{ return _ptr + _size; }

		T * end() const
		{ return _ptr + _size; }

		bool IsEmpty() const
		{ return empty(); }

		bool empty() const
		{ return _size == 0; }

		size_t GetSize() const
		{ return size(); }

		size_t size() const
		{ return _size; }

		template<typename U>
		BasicBuffer<U> Reinterpret() const
		{
			size_t byteSize = size() * sizeof(T);
			if (byteSize % sizeof(U))
				throw Exception("unaligned sizes");
			return BasicBuffer<U>(reinterpret_cast<U*>(_ptr), byteSize / sizeof(U));
		}
	};

	using Buffer		= BasicBuffer<u8>;
	using ConstBuffer	= BasicBuffer<const u8>;

}

#endif
