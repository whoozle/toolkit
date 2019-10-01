#ifndef TOOLKIT_TEXT_TOSTRING_H
#define TOOLKIT_TEXT_TOSTRING_H

#include <toolkit/text/StringOutputStream.h>

namespace TOOLKIT_NS { namespace text
{

	template <typename T>
	std::string ToString(T && value)
	{
		StringOutputStream ss;
		ss << value;
		return ss.Get();
	}

}}

#endif
