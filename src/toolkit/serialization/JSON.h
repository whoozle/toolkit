#ifndef TOOLKIT_SERIALIZATION_JSON_H
#define TOOLKIT_SERIALIZATION_JSON_H

#include <toolkit/core/types.h>
#include <toolkit/serialization/Serialization.h>
#include <string>
#include <tuple>

TOOLKIT_NS_BEGIN
namespace serialization
{

	template<typename ClassType, typename ClassDescriptor>

	class JSONWriter
	{
		ClassDescriptor		_descriptor;
	public:
		JSONWriter(const ClassDescriptor & descriptor) : _descriptor(descriptor)
		{ }
		JSONWriter(ClassDescriptor && descriptor) : _descriptor(descriptor)
		{ }
	};

}
TOOLKIT_NS_END

#endif

