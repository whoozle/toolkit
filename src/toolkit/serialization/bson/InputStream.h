#ifndef TOOLKIT_SERIALIZATION_BSON_OBJECTOUTPUTSTREAM_H
#define TOOLKIT_SERIALIZATION_BSON_OBJECTOUTPUTSTREAM_H

#include <toolkit/core/types.h>
#include <toolkit/core/Buffer.h>
#include <toolkit/serialization/ISerializationStream.h>
#include <toolkit/serialization/bson/Tag.h>
#include <toolkit/serialization/bson/Number.h>
#include <string>
#include <deque>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{
	class BaseInputStream : public IInputSerializationStream
	{
	private:
		IInputSerializationStreamPtr _current;

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
		bool _started;

	public:
		ObjectInputStream(): BaseInputStream()
		{ }

	};

}}}

#endif
