#ifndef TOOLKIT_TEXT_STRING_H
#define TOOLKIT_TEXT_STRING_H

#include <toolkit/core/MemberCheck.h>
#include <toolkit/core/core.h>
#include <string>

namespace TOOLKIT_NS { namespace text
{
	DECLARE_METHOD_CHECK(ToString)

	std::wstring FromUtf8(const std::string &text);
	std::string ToUtf8(const std::wstring &text);
}}



#endif
