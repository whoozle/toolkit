#ifndef TOOLKIT_LOG_ILOGGINGSINK_H
#define TOOLKIT_LOG_ILOGGINGSINK_H

#include <toolkit/core/types.h>
#include <time.h>
#include <string>
#include <memory>

namespace TOOLKIT_NS { namespace log
{

	enum struct LogLevel
	{
		Trace, Debug, Info, Warning, Error
	};

	inline const char * to_string(LogLevel level)
	{
		switch(level)
		{
		case LogLevel::Trace:
			return "trace";
		case LogLevel::Debug:
			return "debug";
		case LogLevel::Info:
			return "info";
		case LogLevel::Warning:
			return "warning";
		case LogLevel::Error:
			return "error";
		default:
			return "unknown";
		}
	}

	struct ILoggingSink
	{
		virtual ~ILoggingSink() { }
		virtual void Log(LogLevel level, const std::string & logger, const timespec &ts, const std::string &value) = 0;
	};
	TOOLKIT_DECLARE_PTR(ILoggingSink);

}}


#endif
