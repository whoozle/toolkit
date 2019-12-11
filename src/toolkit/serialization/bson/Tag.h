#ifndef TOOLKIT_SERIALIZATION_BSON_TAG_H
#define TOOLKIT_SERIALIZATION_BSON_TAG_H

#include <toolkit/core/types.h>

namespace TOOLKIT_NS { namespace serialization {

	namespace bson
	{
		enum struct Tag : u8
		{
			Undefined		= 0,
			BooleanFalse	= 1,
			BooleanTrue		= 2,
			Zero			= 3,
			PositiveInteger	= 4,
			NegativeInteger	= 5,
			PositiveNumber	= 6,
			NegativeNumber	= 7,
			String			= 8,
			ListBegin		= 9,
			ListEnd			= 10,
			ObjectBegin		= 11,
			ObjectKey		= 12,
			ObjectEnd		= 13,
			Null			= 14,
		};
	}

}}

#endif
