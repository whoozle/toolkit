#ifndef TOOLKIT_SERIALIZATION_BSON_H
#define TOOLKIT_SERIALIZATION_BSON_H

#include <toolkit/core/types.h>
#include <toolkit/serialization/Grammar.h>
#include <toolkit/serialization/bson/InputStream.h>
#include <toolkit/serialization/bson/OutputStream.h>
#include <toolkit/core/ByteArray.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization
{

	template<typename ClassType>
	struct BSON
	{
		static void Write(ByteArray::Storage & outStorage, const ClassType & value)
		{
			auto & descriptor = GrammarDescriptorHolder<ClassType>::Get();
			auto writer = descriptor.CreateWriter(value);
			auto iter = std::back_inserter(outStorage);
			serialization::bson::OutputStream<decltype(iter)> out(iter);
			writer->Write(out);
		}

		static size_t Read(ClassType & value, const ByteArray::Storage & inData)
		{
			auto & descriptor = GrammarDescriptorHolder<ClassType>::Get();
			auto reader = descriptor.CreateReader(value);
			serialization::bson::ObjectInputStream<ClassType> in;
			return reader->Read(in, inData);
		}
	};

}}

#endif
