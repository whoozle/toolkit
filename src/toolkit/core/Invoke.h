#ifndef TOOLKIT_CORE_INVOKE
#define TOOLKIT_CORE_INVOKE

#include <toolkit/core/core.h>
#include <functional>

TOOLKIT_NS_BEGIN

	namespace serialization
	{
		template <typename R, typename ArgumentsType, typename ...Args>
		struct function_invoker
		{
			using FunctionType = std::function<R (Args...)>;

			ArgumentsType	&		Arguments;
			FunctionType			Function;

			void dispatch()
			{ return call(std::index_sequence_for<Args...>{}); }

			template<std::size_t ...S>
			R call(std::index_sequence<S...>)
			{ return Function(std::forward<typename std::tuple_element<S, ArgumentsType>::type>(std::get<S>(Arguments)) ...); }
		};
	}

	template<typename R, typename ArgumentsType, typename ... FuncArgs>
	void Invoke(std::function<R(FuncArgs...)> func, ArgumentsType & tuple)
	{
		serialization::function_invoker<R, ArgumentsType, FuncArgs...> invoker = { tuple, func };
		invoker.dispatch();
	}

TOOLKIT_NS_END

#endif
