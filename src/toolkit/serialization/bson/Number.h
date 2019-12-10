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
		static const FloatType v_1_2 = FloatType(1) / 2;
		static const FloatType v_1_4 = v_1_2 / 2;

		IntegerType result = 0;
		if (value == 0)
			return result;

		IntegerType bit = 1;
		value -= v_1_2;
		do
		{
			if (value >= v_1_4)
			{
				value -= v_1_4;
			}
			else
				result |= bit;

			bit <<= 1;
			value *= 2;
		}
		while(value != 0);

		return result;
	}

	template<typename FloatType, typename IntegerType>
	FloatType DecodeNumber(IntegerType value)
	{
		if (value == 0)
			return 0;

		static const FloatType v_1_2 = FloatType(1) / 2;
		static const FloatType v_1_4 = v_1_2 / 2;

		FloatType result = 0;
		while(value)
		{
			if (!(value & 1))
			{
				result += v_1_2;
			}
			result *= v_1_2;
			value >>= 1;
		}

		return result + v_1_2;
	}


}}

#endif
