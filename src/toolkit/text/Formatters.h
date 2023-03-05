#ifndef TOOLKIT_SERIALISATION_FORMATTERS_H
#define TOOLKIT_SERIALISATION_FORMATTERS_H

#include <toolkit/text/StringOutputStream.h>
#include <toolkit/core/Buffer.h>
#include <string>
#include <type_traits>

namespace TOOLKIT_NS { namespace text
{
	inline char NibbleToHex(u8 v)
	{ return v > 9? v + 'a' - 10: v + '0';}

	class StringOutputStream;

	template<typename ValueType, bool IntegralType = std::is_integral<ValueType>::value>
	struct HexNumber;

	template<typename ValueType>
	struct HexNumber<ValueType, true>
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
				buf[--n] = NibbleToHex(value & 0x0f);
				if (value == 0 && Size - n >= width)
					break;
			}
			ss.Write(buf + n, Size - n);
		}
		TOOLKIT_DECLARE_SIMPLE_TOSTRING()
	};

	template<typename ValueType>
	struct HexNumber<ValueType, false>
	{
		const ValueType & 	Value;
		unsigned			Width;

		HexNumber(const ValueType & value, unsigned width = 0):
			Value(value), Width(width)
		{ }

		void ToString(StringOutputStream & ss) const
		{
			for(auto byte : Value) {
				char buf[2] = { NibbleToHex(byte >> 4), NibbleToHex(byte & 0x0f) };
				ss.Write(buf, 2);
			}
		}
		TOOLKIT_DECLARE_SIMPLE_TOSTRING()
	};

	template<typename ValueType>
	HexNumber<ValueType> Hex(const ValueType &value, unsigned padding = 0)
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
