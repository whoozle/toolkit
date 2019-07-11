#ifndef TOOLKIT_TASK
#define TOOLKIT_TASK

#include <functional>
#include <toolkit/core/core.h>

namespace TOOLKIT_NS
{
	using Task = std::function<void ()>;
}

#endif
