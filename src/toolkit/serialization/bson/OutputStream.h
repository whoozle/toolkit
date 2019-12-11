#ifndef TOOLKIT_SERIALIZATION_BSONWRITER_H
#define TOOLKIT_SERIALIZATION_BSONWRITER_H

#include <toolkit/core/types.h>
#include <toolkit/serialization/ISerializationStream.h>
#include <toolkit/serialization/bson/Tag.h>
#include <toolkit/serialization/bson/Number.h>
#include <toolkit/serialization/bson/BinaryWriter.h>
#include <string>
#include <cmath>
#include <limits>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{

	template<typename IteratorType>
	class OutputStream: private BinaryWriter<IteratorType>, public IOutputStream
	{
		using super = BinaryWriter<IteratorType>;

	public:
		OutputStream(IteratorType iter): super(iter) { }

		void Write(const Undefined &) override
		{ WriteTag(Tag::Undefined); }

		void Write(std::nullptr_t) override
		{ WriteTag(Tag::Null); }

		void Write(bool value) override
		{ WriteTag(value? Tag::BooleanTrue: Tag::BooleanFalse); }

		void Write(s64 value) override
		{
			using Type = s64;
			using UnsignedType = typename std::make_unsigned<Type>::type;

			if (value < 0)
				WriteIntegral<Tag::NegativeInteger, UnsignedType>(static_cast<UnsignedType>(-value));
			else
				WriteIntegral<Tag::PositiveInteger, UnsignedType>(static_cast<UnsignedType>(value));
		}

		void Write(double value) override
		{
			if (value == 0)
			{
				WriteTag(Tag::Zero);
				return;
			}
			else if (value < 0)
			{
				WriteTag(Tag::NegativeNumber);
				value = -value;
			}
			else
				WriteTag(Tag::PositiveNumber);

			int exp;
			auto sign = frexp(value, &exp);
			Write(static_cast<s64>(EncodeNumber<u64, double>(sign)));
			Write(static_cast<s64>(exp));
		}

		void Write(const std::string & value) override
		{
			WriteTag(Tag::String);
			super::WriteBlock(value);
		}

		template<typename IteratorType_>
		void Write(IteratorType_ begin, IteratorType_ end)
		{
			WriteTag(Tag::String);
			super::WriteBlock(begin, end);
		}

		void BeginObject() override
		{ WriteTag(Tag::ObjectBegin); }
		void EndObject() override
		{ WriteTag(Tag::ObjectEnd); }

		void BeginList() override
		{ WriteTag(Tag::ListBegin); }
		void EndList() override
		{ WriteTag(Tag::ListEnd); }

	protected:
		void WriteTag(Tag tag)
		{ *super::_iter++ = static_cast<u8>(tag); }

		template<Tag tag, typename Type>
		void WriteIntegral(Type value)
		{
			if (value != 0)
			{
				WriteTag(tag);
				super::WriteInteger(value);
			}
			else
				WriteTag(Tag::Zero);
		}
	};

}}}

#endif
