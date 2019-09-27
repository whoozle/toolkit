#ifndef TOOLKIT_SERIALIZATION_SERIALIZATION_H
#define TOOLKIT_SERIALIZATION_SERIALIZATION_H

#include <toolkit/core/types.h>
#include <toolkit/io/IStream.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization
{

	template<typename ClassType, typename MemberType>
	struct MemberDescriptor
	{
		using Type = MemberType ClassType::*;
		Type			Pointer;
		std::string		Name;

		MemberDescriptor(Type pointer, std::string name): Pointer(pointer), Name(name)
		{ }

		const MemberType & Get(const ClassType * self) const
		{ return self->*Pointer; }

		MemberType & Get(ClassType * self)
		{ return self->*Pointer; }
	};

	namespace impl
	{
		template<typename ... MemberDescriptors>
		struct ClassDescriptor
		{
			using DataType = std::tuple<MemberDescriptors ...>;
			static constexpr size_t MemberCount = std::tuple_size<DataType>::value;

			std::string 						Name;
			uint								Version;
			DataType							Data;

			ClassDescriptor(const std::string & name, uint version, DataType && data):
				Name(name), Version(version), Data(data)
			{ }

			template<typename ClassType, typename MemberType>
			ClassDescriptor<MemberDescriptors..., MemberDescriptor<ClassType, MemberType>> operator &(const MemberDescriptor<ClassType, MemberType> & md)
			{
				using Next = ClassDescriptor<MemberDescriptors..., MemberDescriptor<ClassType, MemberType>>;
				return Next(Name, Version, std::tuple_cat(Data, std::make_tuple(md)));
			}
		};
	}

	template<typename ClassType, typename MemberType>
	MemberDescriptor<ClassType, MemberType> Member(MemberType ClassType::* pointer, const std::string & name = std::string())
	{ return MemberDescriptor<ClassType, MemberType>(pointer, name); }

	inline impl::ClassDescriptor<> ClassDescriptor(const std::string &name = std::string(), uint version = 0)
	{ return impl::ClassDescriptor<>(name, version, std::make_tuple()); }

	template<typename ClassType, template<typename, typename> class SerializatorType>
	auto MakeSerializator()
	{
		auto descriptor = ClassType::GetClassDescriptor();
		using DescriptorType = decltype(descriptor);
		SerializatorType<ClassType, DescriptorType> serializator(std::move(descriptor));
		return serializator;
	}

}}


#endif

