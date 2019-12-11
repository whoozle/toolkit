#ifndef TOOLKIT_SERIALIZATION_BSON_NUMBER_H
#define TOOLKIT_SERIALIZATION_BSON_NUMBER_H

#include <limits>

namespace TOOLKIT_NS { namespace serialization
{
	/*
		Encodes float number using simple arithmetic coder
		Input value expected to be inside [0.5, 1) interval or zero
	*/

	template<typename IntegerType, typename FloatType>
	IntegerType EncodeNumber(FloatType value)
	{
		static_assert(std::is_unsigned<IntegerType>::value, "IntegerType must be unsigned");
		static const FloatType v_1_2 = FloatType(1) / 2;
		static const FloatType v_1_4 = v_1_2 / 2;

		IntegerType bit = 1;
		IntegerType result = 0;
		if (value == 0)
			return 0;

		value -= v_1_2;

		while(value != 0)
		{
			if (value >= v_1_4)
			{
				result |= bit;
				value -= v_1_4;
			}

			value *= 2;
			bit <<= 1;
		}
		result |= bit;

		return result;
	}

	template<typename FloatType, typename IntegerType>
	FloatType DecodeNumber(IntegerType value)
	{
		static_assert(std::is_unsigned<IntegerType>::value, "IntegerType must be unsigned");

		if (value == 0)
			return 0;

		static const FloatType v_1_2 = FloatType(1) / 2;
		static const FloatType v_1_4 = v_1_2 / 2;

		FloatType result = 0;
		IntegerType bit;
		for(bit = IntegerType(1) << ((sizeof(IntegerType) << 3) - 1); bit != 0; bit >>= 1)
		{
			if (value & bit)
			{
				bit >>= 1;
				break;
			}
		}

		for(; bit > 0; bit >>= 1)
		{
			result *= v_1_2;
			if (value & bit)
				result += v_1_4;
		}

		return result + v_1_2;
	}


}}

#endif
