#ifndef TOOLKIT_IO_IPOLLABLE_H
#define TOOLKIT_IO_IPOLLABLE_H

#include <toolkit/core/core.h>

TOOLKIT_NS_BEGIN
namespace io
{

	struct IPollable
	{
		virtual ~IPollable() = default;
		virtual int GetFileDescriptor() const = 0;
	};

}
TOOLKIT_NS_END

#endif

