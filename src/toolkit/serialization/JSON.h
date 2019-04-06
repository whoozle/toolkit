#ifndef TOOLKIT_SERIALIZATION_JSON_H
#define TOOLKIT_SERIALIZATION_JSON_H

#include <toolkit/core/types.h>
#include <toolkit/serialization/Serialization.h>
#include <string>
#include <tuple>

namespace TOOLKIT_NS { namespace serialization
{

	template<typename ClassType, typename ClassDescriptor>

	class JSONWriter
	{
		ClassDescriptor		_descriptor;

	public:
		JSONWriter(ClassDescriptor && descriptor) : _descriptor(descriptor)
		{ }

		template<typename StreamType>
		void Write(StreamType & stream, const ClassType & object) const
		{
		}
	};

}}


#endif

