#ifndef TOOLKIT_IO_ISERIALPORT_H
#define TOOLKIT_IO_ISERIALPORT_H

#include <toolkit/io/IStream.h>
#include <toolkit/io/IPollable.h>
#include <termios.h>

namespace TOOLKIT_NS { namespace io
{
	struct ISerialPort :
		public virtual IBidirectionalStream,
		public virtual IPollable
	{
		virtual bool GetParityBit() = 0;
		virtual void SetParityBit(bool bit) = 0;
		virtual termios GetState() = 0;
		virtual void SetState(const termios & state) = 0;
	};

	DECLARE_PTR(ISerialPort);
}}


#endif
