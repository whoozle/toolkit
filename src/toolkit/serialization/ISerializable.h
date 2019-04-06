#ifndef TOOLKIT_SERIALIZATION_ISERIALIZABLE_H
#define TOOLKIT_SERIALIZATION_ISERIALIZABLE_H

#include <toolkit/core/core.h>

namespace TOOLKIT_NS
{
	struct ISerializable
	{
		virtual ~ISerializable() = default;
	};
}

#endif

