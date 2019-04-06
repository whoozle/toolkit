#ifndef TOOLKIT_ITERABLE_H
#define TOOLKIT_ITERABLE_H

#include <type_traits>
#include <toolkit/core/core.h>

namespace TOOLKIT_NS
{

	template <typename ContainerType>
	class StdContainerIterable
	{
		using IteratorType = typename ContainerType::const_iterator;
		IteratorType _begin, _end;

	public:
		StdContainerIterable(IteratorType begin, IteratorType end): _begin(begin), _end(end) { }

		IteratorType begin() const
		{ return _begin; }
		IteratorType end() const
		{ return _end; }
	};

	template<typename ContainerType>
	StdContainerIterable<ContainerType> MakeIterable(const ContainerType & container)
	{
		using DecayedContainerType = typename std::decay<ContainerType>::type;
		return StdContainerIterable<DecayedContainerType>(container.begin(), container.end());
	}

}

#endif
