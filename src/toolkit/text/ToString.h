#ifndef TOOLKIT_TEXT_TOSTRING_H
#define TOOLKIT_TEXT_TOSTRING_H

#include <toolkit/text/StringOutputStream.h>
#include <toolkit/core/MemberCheck.h>


namespace TOOLKIT_NS { namespace text
{
	DECLARE_METHOD_CHECK(ToString);

	template <typename Stream, typename Type>
	typename std::enable_if<!HasMethod_ToString<Type>::Value, void>::type ToString(Stream & stream, Type && value)
	{ stream << value; }

	template <typename Stream, typename Type>
	typename std::enable_if<HasMethod_ToString<Type>::Value, void>::type ToString(Stream & stream, Type && value)
	{ value.ToString(stream); }

	template <typename Type>
	std::string ToString(Type && value)
	{
		StringOutputStream ss;
		ToString(ss, value);
		return ss.Get();
	}

}}

#endif
