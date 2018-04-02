#ifndef TOOLKIT_CORE_MATH_H
#define TOOLKIT_CORE_MATH_H

#include <toolkit/core/types.h>
#include <type_traits>

TOOLKIT_NS_BEGIN

	//add type checks for integral types here

	template<typename T>
	inline bool IsEven(T v)
	{ return (v % 2) == 0; }

	template<typename T>
	inline T PowOf2(T v)
	{ return pow(2, v); }

	inline uint PowOf2(uint v)
	{ return 1 << v; }

	template<typename T>
	inline T Div2(T v)
	{ return v / 2; }

	template<typename T>
	inline T GCD(T a, T b)
	{
		uint d = 0;
		while (IsEven(a) && IsEven(b))
		{
			a = Div2(a);
			b = Div2(b);
			++d;
		}
		while (a != b)
		{
			if (IsEven(a))
				a = Div2(a);
			else if (IsEven(b))
				b = Div2(b);
			else if (a > b)
				a = Div2(a - b);
			else
				b = Div2(b - a);
		}
		return a * PowOf2(d);
	}

	template<typename T>
	inline T LCM(T a, T b)
	{ return a * b / GCD(a, b); } //fixme: double type here

TOOLKIT_NS_END

#endif
