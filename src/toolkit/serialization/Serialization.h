#ifndef TOOLKIT_SERIALIZATION_SERIALIZATION_H
#define TOOLKIT_SERIALIZATION_SERIALIZATION_H

#include <toolkit/core/types.h>
#include <toolkit/io/IStream.h>
#include <string>

TOOLKIT_NS_BEGIN
namespace serialization
{

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

	namespace impl
	{
		template<typename ... MemberDescriptors>
		struct ClassDescriptor
		{
			std::string 	Name;
			uint			Version;

			ClassDescriptor(const std::string & name, uint version = 0):
				Name(name), Version(version)
			{ }

			template<typename ClassType, typename MemberType>
			ClassDescriptor<MemberDescriptors..., MemberDescriptor<ClassType, MemberType>> operator &(const MemberDescriptor<ClassType, MemberType> & md)
			{
				using Next = ClassDescriptor<MemberDescriptors..., MemberDescriptor<ClassType, MemberType>>;
				return Next(Name, Version);
			}
		};
	}

	template<typename ClassType, typename MemberType>
	MemberDescriptor<ClassType, MemberType> Member(MemberType ClassType::* pointer, const std::string & name = std::string())
	{ return MemberDescriptor<ClassType, MemberType>(pointer, name); }

	impl::ClassDescriptor<> ClassDescriptor(const std::string &name, uint version)
	{ return impl::ClassDescriptor<>(name, version); }


}
TOOLKIT_NS_END

#endif

