#ifndef TOOLKIT_EXCEPTION_H
#define TOOLKIT_EXCEPTION_H

#include <stdexcept>
#include <toolkit/core/core.h>

namespace TOOLKIT_NS
{

	using Exception = std::runtime_error;

	struct NullPointerException : Exception
	{
		NullPointerException(const std::string &what = std::string()): Exception(what + ": null pointer exception")
		{ }
	};

	template<typename T>
	T RequireNotNull(T value)
	{
		ASSERT(value, NullPointerException, /**/);
		return value;
	}

}

#endif
