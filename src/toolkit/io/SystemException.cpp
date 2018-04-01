#include <toolkit/io/SystemException.h>
#include <string>

#include <string.h>
#include <errno.h>

TOOLKIT_NS_BEGIN
namespace io
{

	SystemException::SystemException(const std::string &what) throw() : std::runtime_error(what + ": " + GetErrorMessage())
	{ }

	SystemException::SystemException(const std::string &what, int returnCode) throw() : std::runtime_error(what + ": " + GetErrorMessage(returnCode))
	{ }

	std::string SystemException::GetErrorMessage(int returnCode)
	{
		char buf[1024];
#ifdef _GNU_SOURCE
		return std::string(strerror_r(returnCode, buf, sizeof(buf)));
#else
		int r = strerror_r(returnCode, buf, sizeof(buf));
		return std::string(r == 0? buf: "strerror_r() failed");
#endif
	}
	std::string SystemException::GetErrorMessage()
	{ return GetErrorMessage(errno); }

}
TOOLKIT_NS_END
