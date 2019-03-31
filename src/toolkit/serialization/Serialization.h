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

		const MemberType & Get(ClassType * self) const
		{ return self->*Pointer; }

		MemberType & Get(ClassType * self)
		{ return self->*Pointer; }
	};

	struct VersionDescriptor
	{
		uint Version;
		VersionDescriptor(uint version): Version(version) { }
	};


	template<typename ClassType, typename MemberType>
	MemberDescriptor<ClassType, MemberType> Member(MemberType ClassType::* pointer, const std::string & name = std::string())
	{ return MemberDescriptor<ClassType, MemberType>(pointer); }

	inline VersionDescriptor Version(uint version)
	{ return VersionDescriptor(version); }


}
TOOLKIT_NS_END

#endif

