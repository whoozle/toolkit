#ifndef TOOLKIT_NONCOPYABLE_H
#define TOOLKIT_NONCOPYABLE_H

#include <toolkit/core/core.h>

TOOLKIT_NS_BEGIN

	struct Noncopyable
	{
	protected:
		Noncopyable() = default;

	private:
		Noncopyable(const Noncopyable &) = delete;
		Noncopyable & operator = (const Noncopyable &) = delete;
	};

TOOLKIT_NS_END

#endif
