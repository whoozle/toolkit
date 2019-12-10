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
		while(value != 0)
		{
			if (value >= v_1_4)
			{
				value -= v_1_4;
				result |= bit;
			}
			bit <<= 1;
			value *= 2;
		}
		return result;
	}

	template<typename FloatType, typename IntegerType>
	FloatType DecodeNumber(IntegerType value)
	{
		static const auto eps = std::numeric_limits<FloatType>::epsilon();
		static const FloatType v_1_2 = FloatType(1) / 2;
		static const FloatType v_1_4 = v_1_2 / 2;
		static const int Bits = sizeof(IntegerType) << 3;

		FloatType result = 0;
		IntegerType bit = IntegerType(1) << (Bits - 1 - __builtin_clz(value));
		while(bit)
		{
			if (value & bit)
			{
				result += v_1_2;
			}
			result *= v_1_2;
			bit >>= 1;
		}

		return result + v_1_2;
	}


}}

#endif
