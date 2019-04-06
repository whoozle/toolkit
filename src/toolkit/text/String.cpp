#include <toolkit/text/String.h>
#include <codecvt>
#include <locale>

namespace TOOLKIT_NS { namespace text
{
	namespace
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> Convert;
	}

	std::wstring FromUtf8(const std::string &text)
	{
		return Convert.from_bytes(text);
	}
	
	std::string ToUtf8(const std::wstring &text)
	{
		return Convert.to_bytes(text);
	}

}}

