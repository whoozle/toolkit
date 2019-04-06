#ifndef TOOLKIT_TEXT_HASH_H
#define TOOLKIT_TEXT_HASH_H

#include <toolkit/core/core.h>
#include <string>

namespace TOOLKIT_NS { namespace text
{

#if __SIZEOF_WCHAR_T__ != 4
#	error invalid wchar size
#endif

	//superfast hash
	//http://www.azillionmonkeys.com/qed/hash.html
	//reimplemented for 32 bit wchar_t

	namespace impl
	{
		inline u32 HashAvalanche(u32 hash)
		{
			hash ^= hash << 3;
			hash += hash >> 5;
			hash ^= hash << 4;
			hash += hash >> 17;
			hash ^= hash << 25;
			hash += hash >> 6;
			return hash;
		}
	}

	u32 Hash(const std::wstring &src)
	{
		using CharType = typename std::wstring::value_type;
		u32 hash = src.size() * sizeof(CharType);
		for(auto ch : src)
		{
			hash += ch & 0xffffu;
			auto tmp = ((ch >> 16) << 11) ^ hash;
			hash = (hash << 16) ^ tmp;
			hash += (hash >> 11);
		}
		return impl::HashAvalanche(hash);
	}

}}



#endif
