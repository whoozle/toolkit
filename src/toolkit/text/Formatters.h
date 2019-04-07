#ifndef TOOLKIT_SERIALISATION_FORMATTERS_H
#define TOOLKIT_SERIALISATION_FORMATTERS_H

#include <toolkit/text/StringOutputStream.h>

namespace TOOLKIT_NS { namespace text
{

	template<typename ValueType>
	struct HexNumber
	{
		ValueType 	Value;
		unsigned	Width;

		HexNumber(ValueType value, unsigned width = 0):
			Value(value), Width(width)
		{ }

		void ToString(StringOutputStream & ss) const
		{
			static constexpr size_t Size = sizeof(ValueType) * 2;

			auto value = Value;
			auto width = Width;
			char buf[Size];

			size_t n;
			for(n = Size; n--; value >>= 4)
			{
				char ch = value & 0x0f;
				if (ch > 9)
					ch += 39;
				buf[n] = ch + '0';
				if (value == 0 && Size - n >= width)
					break;
			}
			ss.Write(buf + n, Size - n);
		}
	};

	template<typename ValueType>
	HexNumber<ValueType> Hex(ValueType value, unsigned padding = 0)
	{ return HexNumber<ValueType> { value, padding }; }

}}

#endif
