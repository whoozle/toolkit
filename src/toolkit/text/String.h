#ifndef TOOLKIT_TEXT_STRING_H
#define TOOLKIT_TEXT_STRING_H

#include <toolkit/core/core.h>
#include <toolkit/text/StringOutputStream.h>
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
		}
		return os.Get();
	}

}}



#endif
