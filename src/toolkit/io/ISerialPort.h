#ifndef TOOLKIT_IO_ISERIALPORT_H
#define TOOLKIT_IO_ISERIALPORT_H

#include <toolkit/io/IStream.h>
#include <toolkit/io/IPollable.h>

namespace TOOLKIT_NS { namespace io
{
	struct ISerialPort :
		public virtual IBidirectionalStream,
		public virtual IPollable
	{
		virtual bool GetParityBit() = 0;
		virtual void SetParityBit(bool bit) = 0;
	};

	DECLARE_PTR(ISerialPort);
}}


#endif
