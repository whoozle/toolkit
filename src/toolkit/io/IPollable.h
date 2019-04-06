#ifndef TOOLKIT_IO_IPOLLABLE_H
#define TOOLKIT_IO_IPOLLABLE_H

#include <toolkit/core/core.h>

namespace TOOLKIT_NS { namespace io
{

	struct IPollable
	{
		virtual ~IPollable() = default;
		virtual int GetFileDescriptor() const = 0;
	};

}}


#endif

