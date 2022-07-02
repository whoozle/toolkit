#ifndef TOOLKIT_SERIALIZATION_SERIALIZATION_H
#define TOOLKIT_SERIALIZATION_SERIALIZATION_H

#include <toolkit/core/types.h>
#include <toolkit/core/Exception.h>
#include <toolkit/core/Hash.h>
#include <toolkit/io/IStream.h>
#include <mutex>
#include <string>
#include <unordered_map>

namespace TOOLKIT_NS { namespace serialization
{
	struct TypeDescriptor
	{
		std::string 						Name;
		uint								Version;
		TypeDescriptor(const std::string & name, uint version = 0):
			Name(name), Version(version)
		{ }

		bool operator==(const TypeDescriptor & o) const
		{ return Name == o.Name && Version == o.Version; }
		bool operator!=(const TypeDescriptor & o) const
		{ return !(*this == o); }

		struct Hash
		{
			size_t operator()(const TypeDescriptor & descriptor) const
			{ return CombineHash(descriptor.Name, descriptor.Version); }
		};
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

	class ClassDescriptorRegistry final
	{
	private:
		mutable std::mutex			_mutex;
		std::unordered_map<TypeDescriptor, const IClassDescriptor *, TypeDescriptor::Hash> _registry;

	protected:
		ClassDescriptorRegistry()
		{ }

	public:
		static ClassDescriptorRegistry & Get()
		{
			static ClassDescriptorRegistry registry;
			return registry;
		}

		const IClassDescriptor & GetDescriptor(const TypeDescriptor & descriptor) const
		{
			std::lock_guard<decltype(_mutex)> l(_mutex);
			auto i = _registry.find(descriptor);
			if (i != _registry.end())
				return *i->second;

			THROW(Exception, "no type " + descriptor.Name + " registered");
		}

		void Register(const TypeDescriptor & type, const IClassDescriptor & descriptor)
		{
			std::lock_guard<decltype(_mutex)> l(_mutex);
			auto i = _registry.find(type);
			if (i != _registry.end() && i->second != &descriptor)
				THROW(Exception, "double-registration of type " + type.Name);

			_registry[type] = &descriptor;
		}

		template<typename ClassType>
		void Register()
		{
			auto & descriptor = ClassDescriptorHolder<ClassType>::Get();
			Register(descriptor.Type, descriptor);
		}
	};

}}

TOOLKIT_DECLARE_STD_HASH(TOOLKIT_NS :: serialization:: TypeDescriptor, TOOLKIT_NS :: serialization:: TypeDescriptor::Hash)

#endif

