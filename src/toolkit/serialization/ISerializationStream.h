#ifndef TOOLKIT_ISERIALIZATIONSTREAM_H
#define TOOLKIT_ISERIALIZATIONSTREAM_H

#include <toolkit/core/types.h>
#include <toolkit/io/IStream.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization
{
	struct Undefined { };

	struct ISerializationStream
	{
		virtual ~ISerializationStream() = default;

		virtual void Write(const Undefined &) = 0;
		virtual void Write(std::nullptr_t) = 0;

		virtual void Write(bool value) = 0;
		virtual void Write(s64 value) = 0;
		virtual void Write(double value) = 0;
		virtual void Write(const std::string & value) = 0;

		virtual void BeginList() = 0;
		virtual void EndList() = 0;

		virtual void BeginObject() = 0;
		virtual void EndObject() = 0;
	};
	TOOLKIT_DECLARE_PTR(ISerializationStream);

	struct IInputSerializationStream : public virtual ISerializationStream
	{
		virtual void Parse(ConstBuffer data, size_t & offset) = 0;
		virtual bool Finished() const = 0;
	};

	struct IInputStreamParser : public virtual IInputSerializationStream
	{
		virtual ~IInputStreamParser() = default;
		virtual void Set(ISerializationStream & target) = 0;
	};
	TOOLKIT_DECLARE_PTR(IInputStreamParser);

	TOOLKIT_DECLARE_PTR(IInputSerializationStream);

}}

#endif
