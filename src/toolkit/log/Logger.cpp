#include <toolkit/log/Logger.h>
#include <toolkit/log/ConsoleLoggingSink.h>

namespace TOOLKIT_NS { namespace log
{

	ILoggingSinkPtr LogDispatcher::GetDefaultSink()
	{
		static ILoggingSinkPtr defaultSink(std::make_shared<ConsoleLoggingSink>());
		return defaultSink;
	}

}}

