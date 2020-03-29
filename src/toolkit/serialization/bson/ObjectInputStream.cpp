#include <toolkit/serialization/bson/ObjectInputStream.h>
#include <toolkit/text/Formatters.h>

namespace TOOLKIT_NS { namespace serialization { namespace bson
{


	void BaseObjectInputStream::Parse(ConstBuffer data, size_t & offset)
	{
		size_t size = data.size();
		while(!_finished && offset < size)
		{
			ParseGeneric(data, offset); //read property
			Tokenizer::Parse(data, offset);
		}
	}

	void ObjectMetadataStreamParser::Set(ISerializationStream & target)
	{
		dynamic_cast<GenericObjectInputStream &>(target).CreateObject(TypeDescriptor(Name, Version));
	}

}}}
