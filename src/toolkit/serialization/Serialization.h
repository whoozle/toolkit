#ifndef TOOLKIT_SERIALIZATION_SERIALIZATION_H
#define TOOLKIT_SERIALIZATION_SERIALIZATION_H

#include <toolkit/core/types.h>
#include <toolkit/io/IStream.h>
#include <string>

TOOLKIT_NS_BEGIN
namespace serialization
{
	template<typename Type>
	struct Serializator;

	template<typename ClassType, typename MemberType>
	struct MemberDescriptor
	{
		using Type = MemberType ClassType::*;
		Type			Pointer;
		std::string		Name;

		MemberDescriptor(Type pointer, std::string name): Pointer(pointer), Name(name)
		{ }

		void Write(ClassType *self, io::IOutputStream & stream)
		{ Serializator<MemberType>::Write(stream, self->*Pointer); }

		void Read(ClassType *self, io::IInputStream & stream)
		{ self->*Pointer = Serializator<MemberType>::Read(stream); }
	};

	template<typename ClassType, typename MemberType>
	MemberDescriptor<ClassType, MemberType> Member(MemberType ClassType::* pointer, const std::string & name = std::string())
	{ return MemberDescriptor<ClassType, MemberType>(pointer); }

}
TOOLKIT_NS_END

#endif

