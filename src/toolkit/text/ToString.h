#ifndef TOOLKIT_TEXT_TOSTRING_H
#define TOOLKIT_TEXT_TOSTRING_H

#include <toolkit/text/StringOutputStream.h>
#include <optional>

namespace TOOLKIT_NS { namespace text
{

	template <typename Type>
	std::string ToString(const Type & value)
	{
		StringOutputStream ss;
		ss << value;
		return ss.Get();
	}

	template <typename Stream>
	Stream & operator << (Stream & stream, const std::type_info & ti)
	{ stream << "type_info { " << ti.name() << " }"; return stream; }

	template <typename Stream, typename Type>
	Stream & operator << (Stream & stream, const std::optional<Type> & opt)
	{
		stream << "optional { ";
		if (opt)
			stream << *opt << " ";
		stream << "} ";
		return stream;
	}

}}

#endif
