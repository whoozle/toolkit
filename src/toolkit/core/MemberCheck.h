#ifndef TOOLKIT_CORE_MEMBERCHECK_H
#define TOOLKIT_CORE_MEMBERCHECK_H

#include <toolkit/core/core.h>
#include <type_traits>

namespace TOOLKIT_NS
{

	namespace impl
	{
		template<typename T, bool IC = std::is_class<T>::value>
		struct HasMethodBase : public T
		{ };

		template<typename T>
		struct HasMethodBase<T, false>
		{ };
	}

}

#define DECLARE_METHOD_CHECK(NAME) \
	template<typename T> \
	class HasMethod_##NAME \
	{ \
	struct Fallback { int NAME; }; \
\
	struct Derived : TOOLKIT_NS ::impl::HasMethodBase<T>, Fallback { }; \
	template<typename U, U> struct Check; \
\
	template<typename U>  \
	static constexpr std::false_type func(Check<int Fallback::*, &U::NAME> *) { return std::false_type(); } \
\
	template<typename U>  \
	static constexpr std::true_type func(...) { return std::true_type(); } \
\
public: \
	static constexpr bool Value = decltype(func<Derived>(0))::value; \
	};

#endif /* MEMBERCHECK_H */

