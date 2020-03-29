#ifndef TOOLKIT_SERIALIZATION_BSON_OBJECTOUTPUTSTREAM_H
#define TOOLKIT_SERIALIZATION_BSON_OBJECTOUTPUTSTREAM_H

#include <toolkit/core/types.h>
#include <toolkit/core/Buffer.h>
#include <toolkit/serialization/ISerializationStream.h>
#include <toolkit/serialization/Grammar.h>
#include <toolkit/serialization/bson/Tag.h>
#include <toolkit/serialization/bson/Number.h>
#include <string>
#include <deque>
#include <stack>

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
		void Write(const Undefined &) override;
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

	template<typename Type>
	class IntegralStreamParser : public Tokenizer
	{
	protected:
		Type	_value;
		bool	_negative;

	public:
		IntegralStreamParser(bool negative = false): _value(), _negative(negative) { }

		void Set(ISerializationStream & target) override
		{ target.Write(_value); }
	};

	class IntegerStreamParser : public IntegralStreamParser<s64>
	{
	public:
		using IntegralStreamParser::IntegralStreamParser;

		void Parse(ConstBuffer data, size_t & offset) override;
	};

	class NumberStreamParser : public IntegralStreamParser<double>
	{
	public:
		using IntegralStreamParser::IntegralStreamParser;

		void Parse(ConstBuffer data, size_t & offset) override
		{ _finished = true; }
	};

	class StringStreamParser : public Tokenizer
	{
		IntegerStreamParser 	_lengthParser;
		bool					_lengthParsed;
		size_t					_length;
		std::string 			_value;

	public:
		StringStreamParser(): _lengthParser(false), _lengthParsed(false), _length(0)
		{ }

		void Parse(ConstBuffer data, size_t & offset) override;
		void Write(s64 value) override
		{ _length = value; }
		void Set(ISerializationStream & target) override
		{ target.Write(_value); }
	};

}}}

#endif
