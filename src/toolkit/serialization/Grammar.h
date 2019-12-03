#ifndef TOOLKIT_SERIALIZATION_GRAMMAR_H
#define TOOLKIT_SERIALIZATION_GRAMMAR_H

#include <toolkit/serialization/Serialization.h>
#include <unordered_map>

namespace TOOLKIT_NS { namespace serialization
{
	class GrammarDescriptor
	{
	public:
		template <typename DescriptorsType>
		GrammarDescriptor(const DescriptorsType & descriptor)
		{

		}

		template<typename DescriptorType>
		void AddDescriptor(const DescriptorType & desc) const
		{
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
