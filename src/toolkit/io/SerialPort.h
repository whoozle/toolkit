#ifndef TOOLKIT_SERIAL_PORT_H
#define TOOLKIT_SERIAL_PORT_H

#include <toolkit/io/File.h>
#include <toolkit/io/ISerialPort.h>

TOOLKIT_NS_BEGIN
namespace io
{

	class SerialPort :
		public File,
		public virtual ISerialPort
	{
	public:
		using File::File;

		bool GetParityBit() override;
		void SetParityBit(bool bit) override;
	};

}
TOOLKIT_NS_END

#endif
