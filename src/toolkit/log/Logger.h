#ifndef TOOLKIT_LOG_LOGGER_H
#define TOOLKIT_LOG_LOGGER_H

#include <toolkit/log/ILoggingSink.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/types.h>
#include <toolkit/text/String.h>
#include <toolkit/text/StringOutputStream.h>
#include <list>
#include <string>

#define ANONYMOUS_LOGGER(name) namespace { TOOLKIT_NS :: log :: Logger log(name); }

namespace TOOLKIT_NS { namespace log
{

	class LogDispatcher : public ILoggingSink, Noncopyable
	{
		std::list<std::pair<LogLevel, ILoggingSinkPtr>> _sinks;
		ILoggingSinkPtr 	_defaultSink;
		LogLevel 			_level;
		bool 				_enabled;

	private:
		ILoggingSinkPtr GetDefaultSink();

	public:
		LogDispatcher():
		_sinks(), _defaultSink(),
		_level(LogLevel::Trace), _enabled(true)
		{ }

		void RegisterSink(ILoggingSinkPtr sink, LogLevel level)
		{ _sinks.push_back(std::make_pair(level, sink)); }

		void Log(LogLevel level, const std::string & logger, const timespec &ts, const std::string &value) override
		{
			if (!_enabled || level < _level)
				return;

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

		void SetLogLevel(LogLevel level)
		{ _level = level; }

		void Enable(bool enable)
		{ _enabled = enable; }
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
			NullLogProxy & operator << (const ValueType &value)
			{ return *this; }
		};

		template<LogLevel Level>
		class LogProxy
		{
			const std::string &			_loggerName;
			text::StringOutputStream	_ss;
			timespec					_ts;

		public:
			LogProxy(const std::string & loggerName): _loggerName(loggerName)
			{ clock_gettime(CLOCK_REALTIME, &_ts); }
			LogProxy(const LogProxy &o): _loggerName(o._loggerName), _ts(o._ts)
			{ }

			template<typename ValueType>
			typename std::enable_if<!text::HasMethod_ToString<ValueType>::Value, LogProxy &>::type operator << (const ValueType &value)
			{
				_ss << value;
				return *this;
			}

			template<typename ValueType>
			typename std::enable_if<text::HasMethod_ToString<ValueType>::Value, LogProxy &>::type operator << (const ValueType &value)
			{
				value.ToString(_ss);
				return *this;
			}

			LogProxy & operator << (const std::wstring &value)
			{
				_ss << text::ToUtf8(value);
				return *this;
			}

			~LogProxy()
			{ LogManager::Get().Log(Level, _loggerName, _ts, _ss.Get()); }
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

}}


#endif
