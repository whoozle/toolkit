#ifndef TOOLKIT_SERIALIZATION_BSON_H
#define TOOLKIT_SERIALIZATION_BSON_H

#include <toolkit/core/types.h>
#include <toolkit/serialization/Grammar.h>
#include <toolkit/serialization/ISerializable.h>
#include <toolkit/serialization/bson/InputStream.h>
#include <toolkit/serialization/bson/OutputStream.h>
#include <toolkit/core/ByteArray.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization
{

	struct BSON
	{
		template<typename ClassType>
		static void Write(ByteArray::Storage & outStorage, const ClassType & value)
		{
			auto & descriptor = GrammarDescriptorHolder<ClassType>::Get();
			auto writer = descriptor.CreateWriter(value);
			auto iter = std::back_inserter(outStorage);
			serialization::bson::OutputStream<decltype(iter)> out(iter);
			writer->Write(out);
		}

		static ISerializablePtr Read(const ByteArray::Storage & inData, size_t & r)
		{
			// auto & descriptor = GrammarDescriptorHolder<ClassType>::Get();
			// auto reader = descriptor.CreateReader(value);
			serialization::bson::GenericObjectInputStream in;
			r = 0;
			while (!in.Finished())
				in.Parse(inData, r);
			//r = reader->Read(in, inData);
			return nullptr;
		}
	};

}}

#endif
