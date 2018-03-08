#ifndef TOOLKIT_EXCEPTION_H
#define TOOLKIT_EXCEPTION_H

#include <stdexcept>
#include <toolkit/core/core.h>

TOOLKIT_NS_BEGIN

	using Exception = std::runtime_error;

	struct NullPointerException : Exception
	{
		NullPointerException(const std::string &what = std::string()): Exception(what + ": null pointer exception")
		{ }
	};

	template<typename T>
	T RequireNotNull(T value)
	{
		if (!value)
			throw NullPointerException();
		return value;
	}

TOOLKIT_NS_END

#endif
