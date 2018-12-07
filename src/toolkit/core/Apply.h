#ifndef TOOLKIT_CORE_TUPLEAPPLY_H
#define TOOLKIT_CORE_TUPLEAPPLY_H

#include <toolkit/core/core.h>
#include <tuple>
#include <type_traits>

TOOLKIT_NS_BEGIN

	template<std::size_t I = 0, typename Tuple, typename Func>
	typename std::enable_if< I != std::tuple_size<typename std::decay<Tuple>::type>::value, void >::type
	Apply(Func && func, Tuple && tuple)
	{
		func(std::get<I>(tuple));
		Apply<I + 1>(std::forward<Func>(func), tuple);
	}

	template<std::size_t I = 0, typename Tuple, typename Func>
	typename std::enable_if< I == std::tuple_size<typename std::decay<Tuple>::type>::value, void >::type
	Apply(Func && func, Tuple && tuple)
	{
		// do nothing
	}

	namespace serialization
	{
		template<template <typename> class Transform, typename T, typename Seq = std::make_integer_sequence<int, std::tuple_size<T>::value>>
		struct Transformer;

		template<template <typename> class TransformerType, typename T, int... S>
		struct Transformer<TransformerType, T, std::integer_sequence<int, S...>>
		{
			using Tuple = typename std::decay<T>::type;
			static auto Transform(T & val)
			{
				return std::make_tuple(TransformerType<typename std::tuple_element<S, Tuple>::type>::Transform(std::get<S>(val)) ...);
			}
		};
	}

	template<template <typename> class TransformerType, typename ... Types >
	auto TupleTransform(std::tuple<Types...> & args)
	{
		using Tuple = typename std::decay<std::tuple<Types...>>::type;
		return serialization::Transformer<TransformerType, Tuple>::Transform(args);
	}

TOOLKIT_NS_END

#endif

