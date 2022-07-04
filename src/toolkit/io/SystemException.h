#ifndef TOOLKIT_IO_EXCEPTION_H
#define TOOLKIT_IO_EXCEPTION_H

#include <toolkit/core/Exception.h>

namespace TOOLKIT_NS { namespace io
{

	class SystemException : public Exception
	{
	public:
		SystemException(const std::string &what) throw();
		SystemException(const std::string &what, int retCode) throw();
		static std::string GetErrorMessage(int retCode);
		static std::string GetErrorMessage();
	};

#define SYSTEM_CALL(...) \
	do { auto _retVal = __VA_ARGS__ ; if (_retVal == -1) THROW( TOOLKIT_NS ::io::SystemException, #__VA_ARGS__ ); } while (false)

#define SYSTEM_CALL_RETURN(...) \
	do { auto _retVal = __VA_ARGS__ ; if (_retVal == -1) THROW( TOOLKIT_NS ::io::SystemException, #__VA_ARGS__ ); return _retVal; } while (false)

}}


#endif
