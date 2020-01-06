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
		explicit SerialPort(int fd): File(fd)
		{ }

		SerialPort(const std::string &path, FileOpenMode mode = FileOpenMode::ReadWrite): File(path, mode)
		{ }

		termios GetState() override;
		void SetState(const termios & state) override;

		bool GetParityBit() override;
		void SetParityBit(bool bit) override;
	};

}}


#endif
