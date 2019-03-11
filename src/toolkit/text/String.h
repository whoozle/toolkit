#ifndef TOOLKIT_TEXT_STRING_H
#define TOOLKIT_TEXT_STRING_H

#include <toolkit/core/MemberCheck.h>
#include <toolkit/core/core.h>
#include <string>

TOOLKIT_NS_BEGIN
namespace text
{
	DECLARE_METHOD_CHECK(ToString)

	std::wstring FromUtf8(const std::string &text);
	std::string ToUtf8(const std::wstring &text);
}
TOOLKIT_NS_END


#endif
