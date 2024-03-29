#ifndef TOOLKIT_LOG_LOGGER_H
#define TOOLKIT_LOG_LOGGER_H

#include <toolkit/log/ILoggingSink.h>
#include <toolkit/text/String.h>
#include <toolkit/text/StringOutputStream.h>
#include <toolkit/text/ToString.h>
#include <toolkit/core/Noncopyable.h>
#include <toolkit/core/types.h>
#include <list>
#include <string>

#define TOOLKIT_ANONYMOUS_LOGGER(name) namespace { TOOLKIT_NS :: log :: Logger Log(name); }

namespace TOOLKIT_NS { namespace log
{

	class LogDispatcher : public ILoggingSink, Noncopyable
	{
		std::list<std::pair<LogLevel, ILoggingSinkPtr>> _sinks;
		LogLevel 			_level;
		bool 				_enabled;

	private:
		ILoggingSink & GetDefaultSink();

	public:
		LogDispatcher(LogLevel level = LogLevel::Debug):
			_level(level),
			_enabled(true)
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
				GetDefaultSink().Log(level, logger, ts, value);
		}

		void SetLogLevel(LogLevel level)
		{ _level = level; }

		void Enable(bool enable)
		{ _enabled = enable; }
	};

	class LogManager: public LogDispatcher
	{
	public:
		static LogLevel GetGlobalLogLevel();

		LogManager(): LogDispatcher(GetGlobalLogLevel())
		{ }

		static LogManager & Get();
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

			LogProxy(LogProxy &&o): _loggerName(o._loggerName), _ts(o._ts)
			{ }

			template<typename ValueType>
			LogProxy & operator << (ValueType && value)
			{
				_ss << value;
				return *this;
			}

			~LogProxy()
			{ LogManager::Get().Log(Level, _loggerName, _ts, _ss.Get()); }
		};

		//compile-time no-op proxies
#ifdef TOOLKIT_LOGGER_DISABLE
		template<> class LogProxy<LogLevel::Trace> : public NullLogProxy { using NullLogProxy::NullLogProxy; };
		template<> class LogProxy<LogLevel::Debug> : public NullLogProxy { using NullLogProxy::NullLogProxy; };
		template<> class LogProxy<LogLevel::Info> : public NullLogProxy { using NullLogProxy::NullLogProxy; };
		template<> class LogProxy<LogLevel::Warning> : public NullLogProxy { using NullLogProxy::NullLogProxy; };
		template<> class LogProxy<LogLevel::Error> : public NullLogProxy { using NullLogProxy::NullLogProxy; };
#else
#	ifdef NDEBUG
		template<> class LogProxy<LogLevel::Trace> : public NullLogProxy { using NullLogProxy::NullLogProxy; };
#	endif
#endif
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
