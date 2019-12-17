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

namespace TOOLKIT_NS { namespace serialization { namespace bson
{
	class BaseInputStream : public IInputStreamParser
	{
	protected:
		IInputStreamParserPtr 	_current;
		bool 					_finished;

	protected:
		BaseInputStream(): _finished(false)
		{ }

		bool Parse(ConstBuffer data, size_t & offset) override;

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

		void Set(ISerializationStream & target) override;
	};

	template<typename Type>
	class IntegralStreamParser : public BaseInputStream
	{
	protected:
		Type	_value;
		bool	_negative;
		bool	_loaded;

	public:
		IntegralStreamParser(bool negative = false): _value(), _negative(negative), _loaded(false) { }

		void Set(ISerializationStream & target) override
		{ target.Write(_value); }
	};

	class IntegerStreamParser : public IntegralStreamParser<s64>
	{
	public:
		using IntegralStreamParser::IntegralStreamParser;

		bool Parse(ConstBuffer data, size_t & offset) override;
	};

	class NumberStreamParser : public IntegralStreamParser<double>
	{
	public:
		using IntegralStreamParser::IntegralStreamParser;

		bool Parse(ConstBuffer data, size_t & offset) override
		{ return false; }
	};

	class StringStreamParser : public BaseInputStream
	{
		IntegerStreamParser 	_lengthParser;
		bool					_lengthParsed;
		size_t					_length;
		std::string 			_value;

	public:
		StringStreamParser(): _lengthParser(false), _lengthParsed(false), _length(0)
		{ }

		bool Parse(ConstBuffer data, size_t & offset) override;
		void Write(s64 value) override
		{ _length = value; }
		void Set(ISerializationStream & target) override
		{ target.Write(_value); }
	};


	struct ObjectMetadataStreamParser : public BaseInputStream
	{
		std::string			Name;
		uint				Version;

		void Set(ISerializationStream & target) override
		{
			printf("SET\n");
		}

		void Write(const std::string & value) override
		{ Name = value; }

		void Write(s64 value) override
		{ Version = value; }

		void EndList() override
		{ _finished = true; }
	};

	class BaseObjectInputStream : public BaseInputStream
	{
	protected:
		bool _started;

	public:
		BaseObjectInputStream():
			_started(false)
		{ }

		void BeginObject() override
		{
			if (_started)
				throw Exception("nested object, fixme");
		}

		void EndObject() override
		{ }
	};

	template<typename ClassType>
	class ObjectInputStream : public BaseObjectInputStream
	{
		const GrammarDescriptor<ClassType>  & 	_descriptor;
		std::string								_property;

	public:
		ObjectInputStream():
			_descriptor(GrammarDescriptorHolder<ClassType>::Get())
		{ }

		void Write(const std::string & value) override
		{ _property = value; }

		void BeginList() override
		{
			if (_property == "m") {
				//loading metadata
				_current = std::make_shared<ObjectMetadataStreamParser>();
			}
			else
				throw Exception("unknown object property " + _property);
		}
	};

}}}

#endif
