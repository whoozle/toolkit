#ifndef TOOLKIT_SERIALIZATION_SERIALIZATION_H
#define TOOLKIT_SERIALIZATION_SERIALIZATION_H

#include <toolkit/core/types.h>
#include <toolkit/io/IStream.h>
#include <string>

namespace TOOLKIT_NS { namespace serialization
{
	struct TypeDescriptor
	{
		std::string 						Name;
		uint								Version;
		TypeDescriptor(const std::string & name, uint version = 0):
			Name(name), Version(version)
		{ }
	};

	template<typename ClassType, typename MemberType>
	struct MemberDescriptor
	{
		using Type = MemberType ClassType::*;
		Type			Pointer;
		std::string		Name;

		MemberDescriptor(Type pointer, std::string name): Pointer(pointer), Name(name)
		{ }
	};

	struct IClassDescriptor
	{
		virtual ~IClassDescriptor() = default;
	};

	struct BaseClassDescriptor : IClassDescriptor
	{
		TypeDescriptor						Type;

		BaseClassDescriptor(TypeDescriptor type): Type(type)
		{ }
	};

	namespace impl
	{
		template<typename ... MemberDescriptors>
		struct ClassDescriptor : public BaseClassDescriptor
		{
			using DataType = std::tuple<MemberDescriptors ...>;
			static constexpr size_t MemberCount = std::tuple_size<DataType>::value;

			DataType							Data;

			ClassDescriptor(const TypeDescriptor & type, DataType && data):
				BaseClassDescriptor(type), Data(data)
			{ }

			template<typename ClassType, typename MemberType>
			ClassDescriptor<MemberDescriptors..., MemberDescriptor<ClassType, MemberType>> operator &(const MemberDescriptor<ClassType, MemberType> & md)
			{
				using Next = ClassDescriptor<MemberDescriptors..., MemberDescriptor<ClassType, MemberType>>;
				return Next(Type, std::tuple_cat(Data, std::make_tuple(md)));
			}
		};
	}

	template<typename ClassType, typename MemberType>
	MemberDescriptor<ClassType, MemberType> Member(MemberType ClassType::* pointer, const std::string & name = std::string())
	{ return MemberDescriptor<ClassType, MemberType>(pointer, name); }

	inline impl::ClassDescriptor<> ClassDescriptor(const std::string &name = std::string(), uint version = 0)
	{ return impl::ClassDescriptor<>(TypeDescriptor(name, version), std::make_tuple()); }

	template<typename ClassType>
	struct ClassDescriptorHolder
	{
		static auto & Get()
		{
			static auto descriptor = ClassType::GetClassDescriptor();
			return descriptor;
		}
	};

}}


#endif

