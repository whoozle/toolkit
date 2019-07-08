#include <toolkit/io/SerialPort.h>
#include <toolkit/io/SystemException.h>

#include <termios.h>

namespace TOOLKIT_NS { namespace io
{

	bool SerialPort::GetParityBit()
	{
		termios state;
		SYSTEM_CALL(tcgetattr(_fd, &state));
		return (state.c_cflag & PARODD) == PARODD;
	}

	void SerialPort::SetParityBit(bool bit)
	{
		termios state;
		SYSTEM_CALL(tcgetattr(_fd, &state));

		if (bit)
			state.c_cflag |= PARODD;
		else
			state.c_cflag &= ~PARODD;

		SYSTEM_CALL(tcsetattr(_fd, TCSADRAIN, &state));
	}

	termios SerialPort::GetState()
	{
		termios state;
		SYSTEM_CALL(tcgetattr(_fd, &state));
		return state;
	}

	void SerialPort::SetState(const termios & state)
	{
		SYSTEM_CALL(tcsetattr(_fd, TCSANOW, &state));
	}

}}

