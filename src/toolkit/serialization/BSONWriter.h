#ifndef TOOLKIT_SERIALIZATION_BSONWRITER_H
#define TOOLKIT_SERIALIZATION_BSONWRITER_H

#include <toolkit/core/types.h>
#include <toolkit/serialization/BSON.h>
#include <toolkit/serialization/BinaryWriter.h>
#include <toolkit/serialization/ISerializationStream.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{

	template<typename IteratorType>
	class Writer: private BinaryWriter<IteratorType>, public IOutputStream
	{
		using super = BinaryWriter<IteratorType>;
		IteratorType _iter;

	public:
		Writer(IteratorType iter): _iter(iter) { }

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
			WriteTag(Tag::Number);
			throw std::runtime_error("implement Write(double)");
		}

		void Write(const std::string & value) override
		{
			WriteTag(Tag::String);
			WriteRaw(value);
		}

		template<typename IteratorType_>
		void Write(IteratorType_ begin, IteratorType_ end)
		{
			WriteTag(Tag::String);
			WriteRaw(begin, end);
		}

		template<typename Type>
		void WriteRaw(Type value)
		{ super::Write(value); }

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
		{ *_iter++ = static_cast<u8>(tag); }

		template<Tag tag, typename Type>
		void WriteIntegral(Type value)
		{
			if (value != 0)
			{
				WriteTag(tag);
				WriteRaw<Type>(value);
			}
			else
				WriteTag(Tag::Zero);
		}
	};

}}}

#endif
