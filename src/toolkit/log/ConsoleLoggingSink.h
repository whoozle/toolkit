#ifndef TOOLKIT_LOG_CONSOLELOGGINGSINK_H
#define TOOLKIT_LOG_CONSOLELOGGINGSINK_H

#include <toolkit/log/ILoggingSink.h>

TOOLKIT_SUBNS_BEGIN(log)

	class ConsoleLoggingSink : public ILoggingSink
	{
		bool _console;

	private:
		const char * GetColor(LogLevel level);

	public:
		ConsoleLoggingSink();
		void Log(LogLevel level, const std::string& logger, const timespec& ts, const std::string& value) override;
	};

TOOLKIT_SUBNS_END

#endif
