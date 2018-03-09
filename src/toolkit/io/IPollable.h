#ifndef TOOLKIT_IO_IPOLLABLE_H
#define TOOLKIT_IO_IPOLLABLE_H

#include <toolkit/core/core.h>

TOOLKIT_SUBNS_BEGIN(io)

	struct IPollable
	{
		virtual ~IPollable() = default;
		virtual int GetFileDescriptor() const = 0;
	};

TOOLKIT_SUBNS_END

#endif

