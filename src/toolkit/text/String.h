#ifndef TOOLKIT_TEXT_STRING_H
#define TOOLKIT_TEXT_STRING_H

#include <toolkit/core/MemberCheck.h>
#include <toolkit/core/core.h>
#include <toolkit/text/StringOutputStream.h>
#include <functional>
#include <string>

namespace TOOLKIT_NS { namespace text
{
	DECLARE_METHOD_CHECK(ToString)

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

}}



#endif
