#ifndef ISOCKETEVENTHANDLER_H
#define ISOCKETEVENTHANDLER_H

#include <toolkit/core/core.h>

TOOLKIT_NS_BEGIN
namespace io
{

	struct IPollEventHandler
	{
		virtual ~IPollEventHandler() = default;
		virtual void HandleSocketEvent(int event) = 0;
	};

}
TOOLKIT_NS_END


#endif
