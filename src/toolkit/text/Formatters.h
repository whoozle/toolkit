#ifndef TOOLKIT_SERIALISATION_FORMATTERS_H
#define TOOLKIT_SERIALISATION_FORMATTERS_H

#include <toolkit/text/StringOutputStream.h>

namespace TOOLKIT_NS { namespace text
{

	template<typename ValueType>
	struct HexNumber
	{
		ValueType 	Value;
		unsigned	Padding;

		HexNumber(ValueType value, unsigned padding):
			Value(value), Padding(padding)
		{ }

		void ToString(StringOutputStream & ss) const
		{ }
	};

	template<typename ValueType>
	HexNumber<ValueType> Hex(ValueType value, unsigned padding = 0)
	{ return HexNumber<ValueType> { value, padding }; }

}}

#endif
