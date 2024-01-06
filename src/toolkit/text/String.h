#ifndef TOOLKIT_TEXT_STRING_H
#define TOOLKIT_TEXT_STRING_H

#include <toolkit/core/core.h>
#include <toolkit/text/StringOutputStream.h>
#include <algorithm>
#include <functional>
#include <string>

namespace TOOLKIT_NS { namespace text
{
	std::wstring FromUtf8(const std::string &text);
	std::string ToUtf8(const std::wstring &text);

	template<typename StringType>
	bool StartsWith(const StringType & text, const StringType & substr) {
		auto size = substr.size();
		if (size > text.size())
			return false;

		return text.compare(0, size, substr) == 0;
	}

	template<typename StringType>
	bool EndsWith(const StringType & text, const StringType & substr) {
		auto size = substr.size();
		if (size > text.size())
			return false;

		return text.compare(text.size() - size, size, substr) == 0;
	}

	template<typename StringType>
	void Split(const StringType & text, const StringType & splitstr, std::function<void (StringType &&)> func) {
		std::string::size_type prev = 0;
		auto pos = text.find(splitstr);
		while(pos != text.npos)
		{
			func(text.substr(prev, pos - prev));
			prev = pos + splitstr.size();
			pos = text.find(splitstr, prev);
		}
		func(text.substr(prev));
	}

	template<typename StreamType, typename ContainerType, typename StringType, typename MapFunc = std::function<void (StreamType &, const typename ContainerType::value_type &)> >
	void Join(StreamType & stream, const ContainerType & container, const StringType & delimiter, const MapFunc & mapFunc = MapFunc())
	{
		bool addDelimiter = false;
		for(auto & value : container)
		{
			if (addDelimiter)
				stream << delimiter;
			else
				addDelimiter = true;
			if (mapFunc)
				mapFunc(stream, value);
			else
				stream << value;
		}
	}

	template<typename ContainerType, typename StringType, typename MapFunc = std::function<void (text::StringOutputStream &, const typename ContainerType::value_type &)> >
	std::string Join(const ContainerType & container, const StringType & delimiter, MapFunc mapFunc = MapFunc())
	{
		text::StringOutputStream sos;
		Join(sos, container, delimiter, mapFunc);
		return sos.Get();
	}

	inline std::string Replace(const std::string & text, const std::string & src, const std::string & dst, size_t limit = 0)
	{
		StringOutputStream os;
		std::string::size_type pos = 0;
		while(true)
		{
			auto nextPos = text.find(src, pos);
			if (nextPos == text.npos)
			{
				os << text.substr(pos);
				break;
			}
			os << text.substr(pos, nextPos - pos);
			os << dst;
			pos = nextPos + src.size();
			if (limit && --limit == 0)
			{
				os << text.substr(pos);
				break;
			}
		}
		return os.Get();
	}

	inline std::string Trim(const std::string &str)
	{
	static const char *ws = " \t\f\n\r";
	size_t begin = str.find_first_not_of(ws);
	if (begin == str.npos)
		return {};

	size_t end = str.find_last_not_of(ws);
	if (end == str.npos)
		end = str.size();
	else
		++end;
	return str.substr(begin, end - begin);
	}

	inline std::string ToLower(const std::string &str)
	{
	std::string r(str);
	std::transform(r.begin(), r.end(), r.begin(), ::tolower);
	return r;
	}

	inline std::string ToUpper(const std::string &str)
	{
	std::string r(str);
	std::transform(r.begin(), r.end(), r.begin(), ::toupper);
	return r;
	}

}}



#endif
