#include <toolkit/log/Logger.h>
#include <toolkit/log/ConsoleLoggingSink.h>
#include <toolkit/core/Macros.h>
#include <toolkit/core/Singleton.h>
#include <algorithm>
#include <stdlib.h>
#include <ctype.h>

namespace TOOLKIT_NS { namespace log
{
	LogLevel LogManager::GetGlobalLogLevel() {
		LogLevel level = LogLevel::Debug;
		std::string envVarName = TOOLKIT_STRINGIFY(TOOLKIT_NS) "_LOG_LEVEL";
		std::transform(envVarName.begin(), envVarName.end(), envVarName.begin(), ::toupper);
		char *levelString = getenv(envVarName.c_str());
		if (levelString && levelString[0]) {
			switch(tolower(levelString[0])) {
				case 't': level = LogLevel::Trace; break;
				case 'd': level = LogLevel::Debug; break;
				case 'i': level = LogLevel::Info; break;
				case 'w': level = LogLevel::Warning; break;
				case 'e': level = LogLevel::Error; break;
			}
		}
		return level;
	}

	ILoggingSink & LogDispatcher::GetDefaultSink()
	{
		static Singleton<ConsoleLoggingSink> defaultSink;
		return defaultSink.Get();
	}

	LogManager & LogManager::Get()
	{ static Singleton<LogManager> instance; return instance.Get(); }

}}

