#ifndef TOOLKIT_SERIALIZATION_OBJECTINPUTSTREAM_H
#define TOOLKIT_SERIALIZATION_OBJECTINPUTSTREAM_H

#include <toolkit/serialization/bson/InputStream.h>
#include <toolkit/serialization/Grammar.h>
#include <stack>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{

	struct ObjectMetadataStreamParser : public Tokenizer
	{
		std::string			Name;
		uint				Version;

		void Set(ISerializationStream & target) override;

		void Write(const std::string & value) override
		{ Name = value; }

		void Write(s64 value) override
		{ Version = value; }

		void EndList() override
		{ _finished = true; }
	};

	class BaseObjectInputStream : public Tokenizer
	{
	protected:
		std::stack<IInputStreamParserPtr> 	_stack;
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

		void Parse(ConstBuffer data, size_t & offset) override;
	};

	template<typename ClassType>
	class ObjectRecordInputStream : public Tokenizer
	{
		using Descriptor = const GrammarDescriptor<ClassType>;
		Descriptor  & 	_descriptor;

	public:
		ObjectRecordInputStream(Descriptor & descriptor): _descriptor(descriptor)
		{ }
	};

	class GenericObjectInputStream : public BaseObjectInputStream
	{
		std::string								_property;

	protected:
		void Write(const std::string & value) override
		{ _property = value; }

		void BeginList() override
		{
			if (_property == "m")
			{
				//loading metadata
				_stack.push(std::make_shared<ObjectMetadataStreamParser>());
			}
			else
				throw Exception("unknown object property " + _property);
		}

	public:
		void CreateObject(const TypeDescriptor & type)
		{
			printf("CREATE OBJECT %s:%u\n", type.Name.c_str(), type.Version);
		}
	};

	// template<typename ClassType>
	// class ObjectInputStream : public BaseObjectInputStream
	// {
	// 	const GrammarDescriptor<ClassType>  & 	_descriptor;

	// public:
	// 	ObjectInputStream():
	// 		_descriptor(GrammarDescriptorHolder<ClassType>::Get())
	// 	{ }


	// 	void BeginList() override
	// 	{
	// 		else if (_property == "r")
	// 		{
	// 			_stack.push(std::make_shared<ObjectRecordInputStream<ClassType>>(_descriptor));
	// 		}
	// 		else
	// 			throw Exception("unknown object property " + _property);
	// 	}
	// };

}}}

#endif
