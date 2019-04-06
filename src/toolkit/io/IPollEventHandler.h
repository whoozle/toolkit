#ifndef ISOCKETEVENTHANDLER_H
#define ISOCKETEVENTHANDLER_H

#include <toolkit/core/core.h>

namespace TOOLKIT_NS { namespace io
{

	struct IPollEventHandler
	{
		virtual ~IPollEventHandler() = default;
		virtual void HandleSocketEvent(int event) = 0;
	};

}}



#endif
