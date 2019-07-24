#ifndef TOOLKIT_SERIALISATION_FORMATTERS_H
#define TOOLKIT_SERIALISATION_FORMATTERS_H

#include <toolkit/text/StringOutputStream.h>
#include <toolkit/core/Buffer.h>
#include <string>

namespace TOOLKIT_NS { namespace text
{
	class StringOutputStream;

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
			for(n = Size; n; value >>= 4)
			{
				char ch = value & 0x0f;
				ch = ch > 9? ch + 'a' - 10: ch + '0';
				buf[--n] = ch;
				if (value == 0 && Size - n >= width)
					break;
			}
			ss.Write(buf + n, Size - n);
		}
		TOOLKIT_DECLARE_SIMPLE_TOSTRING()
	};

	template<typename ValueType>
	HexNumber<ValueType> Hex(ValueType value, unsigned padding = 0)
	{ return HexNumber<typename std::decay<ValueType>::type> { value, padding }; }

	struct HexDump
	{
		ConstBuffer 	Buffer;
		std::string 	Name;

		HexDump(ConstBuffer buffer, std::string name = std::string()):
			Buffer(buffer), Name(name)
		{ }

		void ToString(StringOutputStream & ss) const;
		TOOLKIT_DECLARE_SIMPLE_TOSTRING()
	};

}}

#endif
