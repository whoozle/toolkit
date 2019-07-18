#ifndef TOOLKIT_SERIALIZATION_BSON_H
#define TOOLKIT_SERIALIZATION_BSON_H

#include <toolkit/core/types.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{
	enum struct Tag : u8
	{
		Undefined		= 0,
		BooleanFalse	= 1,
		BooleanTrue		= 2,
		Zero			= 3,
		PositiveInteger	= 4,
		NegativeInteger	= 5,
		Number			= 6,
		String			= 7,
		ListBegin		= 8,
		ListEnd			= 9,
		ObjectBegin		= 10,
		ObjectKey		= 11,
		ObjectEnd		= 12,
		Null			= 13,
	};
}}}

#endif
