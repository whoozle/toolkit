#ifndef TOOLKIT_NONCOPYABLE_H
#define TOOLKIT_NONCOPYABLE_H

#include <toolkit/core/core.h>

namespace TOOLKIT_NS
{

	struct Noncopyable
	{
	protected:
		Noncopyable() = default;

	private:
		Noncopyable(const Noncopyable &) = delete;
		Noncopyable & operator = (const Noncopyable &) = delete;
	};

}

#endif
