#ifndef TOOLKIT_IO_ISERIALPORT_H
#define TOOLKIT_IO_ISERIALPORT_H

#include <toolkit/io/IStream.h>
#include <toolkit/io/IPollable.h>

TOOLKIT_NS_BEGIN
namespace io
{
	struct ISerialPort :
		public virtual IBidirectionalStream,
		public virtual IPollable
	{ };
}
TOOLKIT_NS_END

#endif
