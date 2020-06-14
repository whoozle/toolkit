#ifndef TOOLKIT_SERIALIZATION_BSON_BINARYWRITER_H
#define TOOLKIT_SERIALIZATION_BSON_BINARYWRITER_H

#include <toolkit/core/ByteArray.h>
#include <toolkit/core/Noncopyable.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization
{

	template<typename IteratorType>
	class BinaryWriter : public Noncopyable
	{
	protected:
		IteratorType _iter;

	public:
		BinaryWriter(IteratorType iter): _iter(iter) { }

		template<typename Type>
		void WriteInteger(Type value)
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

		template<typename ContainerType>
		void WriteBlock(const ContainerType &str)
		{ WriteBlock(str.begin(), str.end()); }

		template<typename IteratorType_>
		void WriteBlock(IteratorType_ begin, IteratorType_ end)
		{
			size_t size = std::distance(begin, end);
			WriteInteger(size);
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
