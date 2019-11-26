#ifndef TOOLKIT_SERIALIZATION_BSON_H
#define TOOLKIT_SERIALIZATION_BSON_H

#include <toolkit/core/types.h>
#include <toolkit/serialization/Serialization.h>
#include <toolkit/serialization/bson/OutputStream.h>
#include <toolkit/core/ByteArray.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization
{

	template<typename ClassType>
	struct BSON
	{
		static void Write(ByteArray::Storage & outStorage, ClassType & value)
		{
			auto & decriptor = ClassDescriptorHolder<ClassType>::Get();
			auto iter = std::back_inserter(outStorage);
			serialization::bson::OutputStream<decltype(iter)> out(iter);
		}
	};

}}

#endif
