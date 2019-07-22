#ifndef TOOLKIT_ISERIALIZATIONSTREAM_H
#define TOOLKIT_ISERIALIZATIONSTREAM_H

#include <toolkit/core/types.h>
#include <toolkit/io/IStream.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization
{
	struct Undefined { };

	struct IOutputStream
	{
		virtual ~IOutputStream() = default;

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

}}

#endif
