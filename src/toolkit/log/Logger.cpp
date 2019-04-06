#include <toolkit/log/Logger.h>
#include <toolkit/log/ConsoleLoggingSink.h>

namespace TOOLKIT_NS { namespace log
{

	ILoggingSinkPtr LogDispatcher::GetDefaultSink()
	{
		if (!_defaultSink)
			_defaultSink = std::make_shared<ConsoleLoggingSink>();
		return _defaultSink;
	}

}}

