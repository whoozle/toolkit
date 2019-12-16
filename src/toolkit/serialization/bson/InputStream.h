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
	class BaseInputStream : public IInputSerializationStream
	{
	protected:
		IInputStreamParserPtr _current;

	protected:
		BaseInputStream()
		{ }

		size_t Parse(ConstBuffer data) override;

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
	};

	class StringInputStream : BaseInputStream
	{
	public:
		StringInputStream(): BaseInputStream()
		{ }
	};

	template<typename ClassType>
	class ObjectInputStream : public BaseInputStream
	{
		const GrammarDescriptor<ClassType>  & _descriptor;
		bool _started;
		bool _finished;

	public:
		ObjectInputStream():
			_descriptor(GrammarDescriptorHolder<ClassType>::Get()),
			_started(false),
			_finished(false)
		{ }

		void BeginObject() override
		{
			if (_started)
				throw Exception("nested object, fixme");
		}

		size_t Parse(ConstBuffer data) override
		{
			if (_finished)
				return 0;

			return BaseInputStream::Parse(data);
		}
	};

}}}

#endif
