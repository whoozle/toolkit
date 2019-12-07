#ifndef TOOLKIT_SERIALIZATION_GRAMMAR_H
#define TOOLKIT_SERIALIZATION_GRAMMAR_H

#include <toolkit/serialization/Serialization.h>
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

	class GrammarDescriptor
	{
		using MemberMap 	= std::unordered_map<std::string, IDescriptorPtr>;
		using MemberList 	= std::vector<IDescriptorPtr>;

		std::string			_name;
		uint				_version;
		MemberMap			_map;
		MemberList			_list;

	private:
		template<typename ClassType, typename MemberType>
		void Add(const MemberDescriptor<ClassType, MemberType> & desc)
		{
			auto grammarDesc = std::make_shared<GrammarMemberDescriptor<ClassType, MemberType>>(desc.Pointer);
			if (desc.Name.empty())
				_list.push_back(grammarDesc);
			else
				_map[desc.Name] = grammarDesc;
		}

	public:
		template <typename DescriptorsType>
		GrammarDescriptor(const DescriptorsType & descriptor): _name(descriptor.Name), _version(descriptor.Version)
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

		template<typename DescriptorType>
		void AddDescriptor(const DescriptorType & desc)
		{
			Add(desc);
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
	};
}}

#endif
