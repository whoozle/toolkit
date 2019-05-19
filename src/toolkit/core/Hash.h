#ifndef TOOLKIT_CORE_HASH_H
#define TOOLKIT_CORE_HASH_H

#include <cstddef>
#include <functional>

namespace TOOLKIT_NS
{

	template<typename IteratorType>
	std::size_t CombineHashRange(IteratorType begin, IteratorType end)
	{
		std::size_t result = 17;
		for(; begin != end; ++begin)
			result = result * 31 + *begin;
		return result;
	}

	template<typename ... Args>
	std::size_t GetHash(Args ... args)
	{
		std::array<std::size_t, sizeof ... (args)> hashes = \
			{ std::hash<typename std::decay<Args>::type>()(args) ... };
		return CombineHashRange(hashes.begin(), hashes.end());
	}

	template<typename ... Args>
	std::size_t CombineHash(Args ... args)
	{
		std::array<std::size_t, sizeof ... (args)> hashes = \
			{ std::hash<typename std::decay<Args>::type>()(args) ... };

		return CombineHashRange(hashes.begin(), hashes.end());
	}

#define TOOLKIT_DECLARE_STD_HASH(TYPE, HASH_TYPE) \
	namespace std \
	{ \
		template<> \
		struct hash< TYPE > \
		{ \
			std::size_t operator() (const TYPE & id) const \
			{ \
				HASH_TYPE hash; \
				return hash(id); \
			} \
		}; \
	}

}

#endif
