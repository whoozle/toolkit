#include <toolkit/log/Logger.h>
#include <toolkit/log/ConsoleLoggingSink.h>

TOOLKIT_NS_BEGIN
namespace log
{

	ILoggingSinkPtr LogDispatcher::GetDefaultSink()
	{
		if (!_defaultSink)
			_defaultSink = std::make_shared<ConsoleLoggingSink>();
		return _defaultSink;
	}

}
TOOLKIT_NS_END
