#include <toolkit/cli/OptionParser.h>
#include <toolkit/core/Exception.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace TOOLKIT_NS { namespace cli
{

void Option::AddTo(OptionParser & parser)
{
	if (!_parser)
	{
		std::stringstream ss;
		ss << "No storage specified for option(s): ";
		for(auto & alias : _aliases)
			ss << alias << " ";
		throw Exception(ss.str());
	}

	for(auto & alias : _aliases)
		parser.AddOption(alias, _parser, _required);
}

void OptionParser::AddOption(const std::string & opt, const IOptionParserPtr & parser, bool required)
{
	if (_opts.find(opt) != _opts.end())
		throw Exception("Duplicate option " + opt);
	_opts[opt] = parser;
	if (required)
		_required.insert(parser.get());
}

std::string OptionParser::GetProgramName(int argc, char ** argv)
{
	if (argc == 0)
		return "(nil)";
	std::string prog = argv[0];
	auto pos = prog.rfind('/');
	if (pos != prog.npos)
		prog = prog.substr(pos + 1);
	return prog;
}

void OptionParser::ShowHelp(int argc, char ** argv)
{
	printf("Usage: %s %s%s\n", GetProgramName(argc, argv).c_str(), !_opts.empty()? "[OPTION]... ": "", "[ARGS]");
}

void OptionParser::Error(int argc, char ** argv, const char *msg, const char *arg)
{
	fprintf(stderr, "%s: %s -- '%s'\n", GetProgramName(argc, argv).c_str(), msg, arg);
	exit(2);
}

void OptionParser::HandleOptions(int argc, char ** argv, IOptionParser & parser, int & i)
{
	OptionList values;
	size_t n = parser.GetArgumentCount();
	if (n == IOptionParser::AnyArgumentCount)
		n = argc - i - 1;
	if (i + 1 + n > argc)
		return Error(argc, argv, "option requires additional argument(s)", argv[i] + 2);
	while(n--)
		values.push_back(argv[1 + i++]);
	parser.FromString(values);
	_required.erase(&parser);
}

void OptionParser::Parse(int argc, char ** argv)
{
	bool handleOptions = true;
	for(int i = 1; i < argc; ++i)
	{
		const char * arg = argv[i];
		if (handleOptions && arg[0] == '-')
		{
			if (arg[1] == '-')
			{
				if (arg[2] == '\0') {
					//Handle '--'
					handleOptions = false;
					continue;
				}
				//Long option
				const char * nameEnd = strchr(arg + 2, '=');
				if (nameEnd)
				{
					std::string name(arg, nameEnd - arg);
					auto it = _opts.find(name);
					if (it == _opts.end())
						return Error(argc, argv, "invalid option", arg + 2);
					auto & parser = it->second;
					if (parser->GetArgumentCount() == 0)
						return Error(argc, argv, "option doesn't allow an argument", arg + 2);

					std::string value(nameEnd + 1);
					parser->FromString({ value });
					_required.erase(parser.get());
				}
				else
				{
					auto it = _opts.find(arg);
					if (it == _opts.end())
						return Error(argc, argv, "invalid option", arg + 2);

					auto & parser = it->second;
					HandleOptions(argc, argv, *parser, i);
				}
			}
			else if (arg[1] == '\0')
			{
				return Error(argc, argv, "invalid argument", arg);
			}
			else
			{
				auto it = _opts.find(arg);
				if (it == _opts.end())
					return Error(argc, argv, "invalid option", arg + 1);
				auto & parser = it->second;
				HandleOptions(argc, argv, *parser, i);
			}
		}
		else
		{
			return Error(argc, argv, "invalid argument", arg);
		}
	}
	if (!_required.empty())
		return Error(argc, argv, "required options are missing", "");
}

}}
