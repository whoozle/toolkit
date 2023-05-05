#ifndef TOOLKIT_SERIALIZATION_BSON_OBJECTOUTPUTSTREAM_H
#define TOOLKIT_SERIALIZATION_BSON_OBJECTOUTPUTSTREAM_H

#include <toolkit/core/types.h>
#include <toolkit/core/Buffer.h>
#include <toolkit/serialization/ISerializationStream.h>
#include <toolkit/serialization/Serializator.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{
	class Tokenizer : public IInputStreamParser
	{
	private:
		IInputStreamParserPtr 				_current;

	protected:
		bool 								_finished;

	public:
		Tokenizer(): _finished(false)
		{ }

		void Parse(ConstBuffer data, size_t & offset) override;

	protected:
		void ParseGeneric(ConstBuffer data, size_t & offset);

	public:
		void Write(Undefined) override;
		void Write(std::nullptr_t) override;
		void Write(bool value) override;
		void Write(s64 value) override;
		void Write(double value) override;
		void Write(const std::string & value) override;
		void BeginList() override;
		void EndList() override;
		void BeginObject() override;
		void EndObject() override;

		bool Finished() const override
		{ return _finished; }
		void Set(ISerializationStream & target) override;
	};

	template<typename ValueType>
	struct SingleValueParser : public Tokenizer
	{
		ValueType 	Value;

		SingleValueParser(): Value() { }

		void Write(typename MapToSerializationType<ValueType>::Type value) override
		{
			Value = static_cast<ValueType>(value);
			_finished = true;
		}
	};

	//helper function in case you have all data available and want to read just single value
	template<typename ValueType>
	ValueType ReadSingleValue(ConstBuffer data, size_t & offset)
	{
		SingleValueParser<ValueType> parser;
		parser.Parse(data, offset);
		ASSERT(parser.Finished(), Exception, "could not read single value (not enough data)");
		return parser.Value;
	}

}}}

#endif
