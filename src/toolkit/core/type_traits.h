#ifndef TOOLKIT_TYPE_TRAITS
#define TOOLKIT_TYPE_TRAITS

#include <toolkit/core/types.h>

namespace TOOLKIT_NS
{


	template<typename T>
	struct DoubleSize;

	template<> struct DoubleSize<u8>  { using Type = u16; };
	template<> struct DoubleSize<u16> { using Type = u32; };
	template<> struct DoubleSize<u32> { using Type = u64; };

	template<> struct DoubleSize<s8>  { using Type = s16; };
	template<> struct DoubleSize<s16> { using Type = s32; };
	template<> struct DoubleSize<s32> { using Type = s64; };

}

#endif

