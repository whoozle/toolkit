#ifndef TOOLKIT_SERIALIZATION_BINARYWRITER_H
#define TOOLKIT_SERIALIZATION_BINARYWRITER_H

#include <toolkit/core/ByteArray.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization
{

	template<typename IteratorType>
	class BinaryWriter
	{
		IteratorType _iter;

	public:
		BinaryWriter(IteratorType iter): _iter(iter) { }

		template<typename Type>
		void Write(Type value)
		{
			//static_assert(std::is_unsigned<Type>::value, "only unsigned types supported");
			if (value < 0)
				throw std::runtime_error("negative values cannot be serialized");

			do
			{
				u8 next = value & 0x7f;
				value >>= 7;
				if (value != 0)
					next |= 0x80;
				*_iter++ = next;
			}
			while(value != 0);
		}

		void Write(std::nullptr_t)
		{ }

		void Write(const std::string &str)
		{ Write(str.begin(), str.end()); }

		template<typename ContainerType>
		void Write(const ContainerType &str)
		{ Write(str.begin(), str.end()); }

		template<typename IteratorType_>
		void Write(IteratorType_ begin, IteratorType_ end)
		{
			size_t size = std::distance(begin, end);
			Write(size);
			_iter = std::copy(begin, end, _iter);
		}
	};

	template<typename ContainerType>
	BinaryWriter<std::back_insert_iterator<ContainerType>> MakeBinaryWriter(ContainerType &data)
	{
		auto iter = std::back_inserter(data);
		return BinaryWriter<decltype(iter)>(iter);
	}

}}

#endif
