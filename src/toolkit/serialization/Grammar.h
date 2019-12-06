#ifndef TOOLKIT_SERIALIZATION_GRAMMAR_H
#define TOOLKIT_SERIALIZATION_GRAMMAR_H

#include <toolkit/serialization/Serialization.h>
#include <unordered_map>
#include <list>
#include <memory>

namespace TOOLKIT_NS { namespace serialization
{
	struct IDescriptor
	{
		virtual ~IDescriptor() = default;
	};
	TOOLKIT_DECLARE_PTR(IDescriptor);
	TOOLKIT_DECLARE_CONST_PTR(IDescriptor);


	template<typename Type>
	struct ITypedDescriptor : public IDescriptor
	{
		virtual ~ITypedDescriptor() = default;
	};

	template<typename ClassType, typename MemberType>
	class GrammarMemberDescriptor final : public ITypedDescriptor<MemberType>
	{
		using Pointer 		= MemberType ClassType::*;

		Pointer				_pointer;

	public:
		GrammarMemberDescriptor(Pointer pointer): _pointer(pointer)
		{ }
	};

	class GrammarDescriptor
	{
		using MemberMap 	= std::unordered_map<std::string, IDescriptorPtr>;
		using MemberList 	= std::list<IDescriptorPtr>;

		MemberMap			_map;
		MemberList			_list;

	private:
		template<typename ClassType, typename MemberType>
		void Add(const MemberDescriptor<ClassType, MemberType> & desc)
		{
			auto grammarDesc = std::make_shared<GrammarMemberDescriptor<ClassType, MemberType>>(desc.Pointer);
			if (desc.Name.empty())
				_list.push_back(desc);
			else
				_map[desc.Name] = desc;
		}

	public:
		template <typename DescriptorsType>
		GrammarDescriptor(const DescriptorsType & descriptor)
		{

		}

		template<typename DescriptorType>
		void AddDescriptor(const DescriptorType & desc) const
		{
			Add(desc);
		}

		template<std::size_t MemberCount, size_t Index = 0, typename DescriptorsType>
		typename std::enable_if<Index < MemberCount, void>::type
		AddDescriptors(const DescriptorsType & descriptors) const
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
