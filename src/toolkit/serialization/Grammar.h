#ifndef TOOLKIT_SERIALIZATION_GRAMMAR_H
#define TOOLKIT_SERIALIZATION_GRAMMAR_H

#include <toolkit/serialization/Serialization.h>
#include <toolkit/serialization/ISerializationStream.h>
#include <unordered_map>
#include <vector>
#include <memory>

namespace TOOLKIT_NS { namespace serialization
{
	struct IDescriptor
	{
		virtual ~IDescriptor() = default;
	};
	TOOLKIT_DECLARE_PTR(IDescriptor);
	TOOLKIT_DECLARE_CONST_PTR(IDescriptor);

	struct IObjectWriter
	{
		virtual ~IObjectWriter() = default;
		virtual void Write(IOutputStream & out) = 0;
	};
	TOOLKIT_DECLARE_PTR(IObjectWriter);

	template<typename ClassType>
	class GrammarDescriptor;

	template<typename ClassType>
	class ObjectWriter : public IObjectWriter
	{
	private:
		using Descriptor = GrammarDescriptor<ClassType>;

		Descriptor &			_descriptor;
		const ClassType &		_object;

	public:
		ObjectWriter(GrammarDescriptor<ClassType> & descriptor, const ClassType & object):
			_descriptor(descriptor), _object(object)
		{ }

		void Write(IOutputStream & out) override
		{ }
	};

	template<typename ClassType>
	class GrammarObjectFactory
	{
		std::string			_name;
		uint				_version;

	public:
		GrammarObjectFactory(const std::string & name, uint version):
			_name(name), _version(version)
		{ }

		IObjectWriterPtr CreateWriter(const ClassType & object) const
		{ return std::make_shared<ObjectWriter<ClassType>>(*this, object); }
	};

	template<typename ClassType, typename MemberType>
	class GrammarMemberDescriptor final : public IDescriptor
	{
		using Pointer 		= MemberType ClassType::*;

		Pointer				_pointer;

	public:
		GrammarMemberDescriptor(Pointer pointer): _pointer(pointer)
		{ }

		const MemberType & Get(const ClassType * self) const
		{ return self->*_pointer; }

		MemberType & Get(ClassType * self)
		{ return self->*_pointer; }
	};

	template<typename ClassType>
	class GrammarDescriptor
	{
		using MemberMap 	= std::unordered_map<std::string, IDescriptorPtr>;
		using MemberList 	= std::vector<IDescriptorPtr>;
		using FactoryPtr    = std::shared_ptr<GrammarObjectFactory<ClassType>>;

		FactoryPtr			_factory;
		MemberMap			_map;
		MemberList			_list;

	private:
		template<typename MemberType>
		void Add(const MemberDescriptor<ClassType, MemberType> & descriptor)
		{
			auto grammarDesc = std::make_shared<GrammarMemberDescriptor<ClassType, MemberType>>(descriptor.Pointer);
			if (descriptor.Name.empty())
				_list.push_back(grammarDesc);
			else
				_map[descriptor.Name] = grammarDesc;
		}

		template<typename DescriptorType>
		void AddDescriptor(const DescriptorType & descriptor)
		{
			Add(descriptor);
		}

		template<std::size_t MemberCount, size_t Index = 0, typename DescriptorsType>
		typename std::enable_if<Index < MemberCount, void>::type
		AddDescriptors(const DescriptorsType & descriptors)
		{
			AddDescriptor(std::get<Index>(descriptors.Data));
			AddDescriptors<MemberCount, Index + 1>(descriptors);
		}

		template<std::size_t MemberCount, size_t Index = 0, typename DescriptorsType>
		static typename std::enable_if<Index == MemberCount, void>::type
		AddDescriptors(const DescriptorsType & descriptors)
		{ }

	public:
		template <typename DescriptorsType>
		GrammarDescriptor(const DescriptorsType & descriptor):
			_factory(std::make_shared<GrammarObjectFactory<ClassType>>(descriptor.Name, descriptor.Version))
		{ AddDescriptors<DescriptorsType::MemberCount>(descriptor); }


		size_t GetListSize() const
		{ return _list.size(); }

		IDescriptorPtr GetMember(size_t index) const
		{ return _list.at(index); }

		IDescriptorPtr GetMember(const std::string & name) const
		{
			auto i = _map.find(name);
			return i != _map.end()? i->second: nullptr;
		}
	};

	template<typename ClassType>
	struct GrammarDescriptorHolder
	{
		static auto & Get()
		{
			static GrammarDescriptor<ClassType> descriptor(ClassDescriptorHolder<ClassType>::Get());
			return descriptor;
		}
	};


}}

#endif
