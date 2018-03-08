#ifndef TOOLKIT_IO_EXCEPTION_H
#define TOOLKIT_IO_EXCEPTION_H

#include <toolkit/core/Exception.h>

TOOLKIT_SUBNS_BEGIN(io)

	class SystemException : public Exception
	{
	public:
		SystemException(const std::string &what) throw();
		SystemException(const std::string &what, int retCode) throw();
		static std::string GetErrorMessage(int retCode);
	};

TOOLKIT_SUBNS_END

#endif
