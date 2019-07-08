#ifndef TOOLKIT_SERIAL_PORT_H
#define TOOLKIT_SERIAL_PORT_H

#include <toolkit/io/File.h>
#include <toolkit/io/ISerialPort.h>

namespace TOOLKIT_NS { namespace io
{

	class SerialPort :
		public File,
		public virtual ISerialPort
	{
	public:
		using File::File;
		SerialPort(const std::string &path, FileOpenMode mode = FileOpenMode::ReadWrite): File(path, mode)
		{ }

		bool GetParityBit() override;
		void SetParityBit(bool bit) override;
	};

}}


#endif
