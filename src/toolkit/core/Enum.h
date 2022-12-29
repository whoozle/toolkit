#ifndef TOOLKIT_CORE_ENUM_H
#define TOOLKIT_CORE_ENUM_H

#include <toolkit/core/core.h>
#include <type_traits>

#define TOOLKIT_DECLARE_ENUM_BIT_OPERATOR(ENUM, OP) \
	inline ENUM operator OP (ENUM a, ENUM b) \
	{ \
		using T = std::underlying_type<ENUM>::type; \
		return ENUM(static_cast<T>(a) OP static_cast<T>(b)); \
	}

#define TOOLKIT_DECLARE_ENUM_BIT_OPERATORS(ENUM) \
	TOOLKIT_DECLARE_ENUM_BIT_OPERATOR(ENUM, |) \
	TOOLKIT_DECLARE_ENUM_BIT_OPERATOR(ENUM, &) \
	TOOLKIT_DECLARE_ENUM_BIT_OPERATOR(ENUM, ^)


#endif
