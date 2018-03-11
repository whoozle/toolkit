#include <toolkit/log/ConsoleLoggingSink.h>
#include <unistd.h>

TOOLKIT_NS_BEGIN
namespace log
{

	ConsoleLoggingSink::ConsoleLoggingSink(): _console(isatty(STDERR_FILENO))
	{
		setlinebuf(stderr);
	}
	
	const char * ConsoleLoggingSink::GetColor(LogLevel level)
	{
		switch(level)
		{
		case LogLevel::Trace:
		case LogLevel::Debug:
			return "\033[30;1m";
		case LogLevel::Warning:
			return "\033[33m";
		case LogLevel::Error:
			return "\033[31;1m";
		default:
			return "";
		}
	}

	void ConsoleLoggingSink::Log(LogLevel level, const std::string& logger, const timespec& ts, const std::string& value)
	{
		if (_console)
			fprintf(stderr, "%s%ld.%03ld: %s[%s]: %s\033[0m\n", GetColor(level), static_cast<long>(ts.tv_sec), static_cast<long>(ts.tv_nsec / 1000000), logger.c_str(), to_string(level), value.c_str());
		else
			fprintf(stderr, "%ld.%03ld: %s[%s]: %s\n", static_cast<long>(ts.tv_sec), static_cast<long>(ts.tv_nsec / 1000000), logger.c_str(), to_string(level), value.c_str());
	}

}
TOOLKIT_NS_END
