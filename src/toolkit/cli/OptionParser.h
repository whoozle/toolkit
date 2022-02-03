#ifndef TOOLKIT_TEXT_ARGUMENT_PARSER_H
#define TOOLKIT_TEXT_ARGUMENT_PARSER_H

#include <toolkit/text/FromString.h>
#include <toolkit/core/types.h>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <optional>
#include <unordered_set>

namespace TOOLKIT_NS { namespace cli
{
	using OptionList = std::list<std::string>;

	struct IOptionParser
	{
		static constexpr int AnyArgumentCount = -1;

		virtual ~IOptionParser() = default;
		virtual int GetArgumentCount() const = 0;
		virtual void FromString(const OptionList & values) = 0;

		virtual void SetName(const std::string & name) = 0;
		virtual void WriteName(text::StringOutputStream & ss) const = 0;
	};
	TOOLKIT_DECLARE_PTR(IOptionParser);

	class OptionParserBase : public IOptionParser
	{
		std::string _name;

	public:
		void SetName(const std::string & name) override
		{ _name = name; }

		void WriteName(text::StringOutputStream & ss) const override
		{ ss << _name; }
	};

	class SimpleOptionParser : public OptionParserBase
	{
	private:
		bool & _value;

	public:
		SimpleOptionParser(bool & value): _value(value)
		{ }

		int GetArgumentCount() const override
		{ return 0; }

		void FromString(const OptionList & values) override
		{ _value = true; }
	};

	template<typename T>
	class SingleOptionParser : public OptionParserBase
	{
	private:
		T & _value;

	public:
		SingleOptionParser(T & value): _value(value)
		{ }

		int GetArgumentCount() const override
		{ return 1; }

		void FromString(const OptionList & values) override
		{ _value = text::FromString<T>(values.front()); }
	};

	template<typename T>
	class IncrementOptionParser : public OptionParserBase
	{
	private:
		T & _value;

	public:
		IncrementOptionParser(T & value): _value(value)
		{ }

		int GetArgumentCount() const override
		{ return 0; }

		void FromString(const OptionList & values) override
		{ ++_value; }
	};

	class SimpleOption : public OptionParserBase
	{
		bool & _value;

	public:
		SimpleOption(bool & value): _value(value)
		{ }

		int GetArgumentCount() const override
		{ return 0; }

		void FromString(const OptionList & values) override
		{ _value = true; }
	};

	class OptionParser
	{
		std::string 							_help;
		std::map<std::string, IOptionParserPtr>	_opts;
		std::list<IOptionParserPtr>				_args;
		std::unordered_set<IOptionParser*>		_required;

	private:
		void Error(int argc, char ** argv, const char *msg, const char *arg);
		void HandleOptions(int argc, char ** argv, IOptionParser & parser, int & i);

	public:
		OptionParser(const std::string & help): _help(help)
		{ }

		static std::string GetProgramName(int argc, char ** argv);

		void Parse(int argc, char ** argv);
		void ShowHelp(int argc, char ** argv);

		void AddOption(const std::string & opt, const IOptionParserPtr & parser, bool required);
	};

	class Option final
	{
		std::set<std::string>	_aliases;
		std::string 			_help;
		bool					_required = false;
		IOptionParserPtr		_parser;

	private:
		void AddAlias()
		{ }

		template<typename Type, typename ... Types>
		void AddAlias(Type && alias, Types && ... aliases)
		{
			_aliases.insert(alias);
			AddAlias(aliases ...);
		}

	public:
		template<typename ... Types>
		Option(Types && ... aliases)
		{ AddAlias(std::forward<Types>(aliases)...); }

		Option & Help(const std::string & help)
		{
			_help = help;
			return *this;
		}

		Option & StoreTrue(bool & value)
		{
			_parser = std::make_shared<SimpleOptionParser>(value);
			return *this;
		}

		template<typename Type>
		Option & Store(Type & value)
		{
			_parser = std::make_shared<SingleOptionParser<Type>>(value);
			_parser->SetName(text::Join(_aliases, "/"));
			return *this;
		}

		template<typename Type>
		Option & Increment(Type & value)
		{
			_parser = std::make_shared<IncrementOptionParser<Type>>(value);
			return *this;
		}

		Option & Required()
		{
			_required = true;
			return *this;
		}

		void AddTo(OptionParser & parser);
	};

}}

#endif
