#ifndef TOOLKIT_SERIALIZATION_ISERIALIZABLE_H
#define TOOLKIT_SERIALIZATION_ISERIALIZABLE_H

#include <toolkit/core/types.h>
#include <memory>

namespace TOOLKIT_NS
{
	struct ISerializableType
	{
		virtual ~ISerializableType() = default;
		virtual const std::string GetType() const = 0;
	};
	TOOLKIT_DECLARE_CONST_PTR(ISerializableType);

	struct ISerializable
	{
		virtual ~ISerializable() = default;
		virtual ISerializableTypeConstPtr GetType() const = 0;
	};
	TOOLKIT_DECLARE_PTR(ISerializable);
}

#endif

