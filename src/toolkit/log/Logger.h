#ifndef TOOLKIT_LOG_LOGGER_H
#define TOOLKIT_LOG_LOGGER_H

#include <toolkit/log/ILoggingSink.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/types.h>
#include <toolkit/text/String.h>
#include <list>
#include <string>
#include <sstream>
#include <stdio.h>

#define ANONYMOUS_LOGGER(name) namespace { TOOLKIT_NS :: log :: Logger log(name); }

TOOLKIT_NS_BEGIN
namespace log
{

	class LogDispatcher : public ILoggingSink
	{
		std::list<std::pair<LogLevel, ILoggingSinkPtr>> _sinks;
		ILoggingSinkPtr _defaultSink;

	private:
		ILoggingSinkPtr GetDefaultSink();

	public:
		void RegisterSink(ILoggingSinkPtr sink, LogLevel level)
		{ _sinks.push_back(std::make_pair(level, sink)); }

		void Log(LogLevel level, const std::string & logger, const timespec &ts, const std::string &value) override
		{
			if (!_sinks.empty())
			{
				for(auto &sl : _sinks)
				{
					if (level >= sl.first)
						sl.second->Log(level, logger, ts, value);
				}
			}
			else
				GetDefaultSink()->Log(level, logger, ts, value);
		}
	};

	class LogManager: public LogDispatcher
	{
	public:
		static LogManager & Get()
		{ static LogManager instance; return instance; }

	private:
		LogManager() { }
	};

	namespace impl
	{
		struct NullLogProxy
		{
			NullLogProxy(const std::string &) { }

			template<typename ValueType>
			NullLogProxy & operator % (const ValueType &value)
			{ return *this; }
		};

		template<LogLevel Level>
		class LogProxy
		{
			const std::string &	_loggerName;
			std::stringstream	_ss;
			timespec			_ts;

		public:
			LogProxy(const std::string & loggerName): _loggerName(loggerName)
			{ clock_gettime(CLOCK_REALTIME, &_ts); }
			LogProxy(const LogProxy &o): _loggerName(o._loggerName), _ts(o._ts)
			{ }

			template<typename ValueType>
			LogProxy & operator << (const ValueType &value)
			{
				_ss << value;
				return *this;
			}

			LogProxy & operator << (const std::wstring &value)
			{
				_ss << text::ToUtf8(value);
				return *this;
			}

			~LogProxy()
			{ LogManager::Get().Log(Level, _loggerName, _ts, _ss.str()); }
		};

		//compile-time no-op proxies
		template<> class LogProxy<LogLevel::Trace> : public NullLogProxy { using NullLogProxy::NullLogProxy; };
	}

	class Logger : Noncopyable
	{
		std::string _name;

	public:
		Logger(const std::string &name): _name(name) { }

#define _DECLARE_LOGGER(LEVEL) impl::LogProxy< LogLevel :: LEVEL > LEVEL ( ) { return impl::LogProxy < LogLevel :: LEVEL > (_name); }

		_DECLARE_LOGGER(Trace)
		_DECLARE_LOGGER(Debug)
		_DECLARE_LOGGER(Info)
		_DECLARE_LOGGER(Warning)
		_DECLARE_LOGGER(Error)

#undef _DECLARE_LOGGER
	};

#define DECLARE_LOGGER(Class) namespace { TOOLKIT_NS::log::Logger _logger(#Class); }

}
TOOLKIT_NS_END

#endif
