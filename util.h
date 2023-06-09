#ifndef _UTIL_H_
#define _UTIL_H_

#include "compiler.h"

namespace bb8
{

class Util
{

	

public:


	static void SwapEndianness(uint8_t* data, uint8_t size)
	{
		for (uint8_t bit = 0; bit < size / 2; ++bit)
			Swap(data + bit, data + size - bit - 1);
	}

	static void Swap(uint8_t* a, uint8_t* b)
	{
		uint8_t tmp = *a;
		*a = *b;
		*b = tmp;
	}
	
	static bool Approximately(float a, float b, float epsilon = FLT_EPSILON)
	{
		return fabs(a - b) <= epsilon;
	}

	static int Sign(int x)
	{
		return (x > 0) - (x < 0);
	}

	static float Sign(float x)
	{
		return x == 0.0f ? 0.0f : (x > 0.0f ? 1.0f : -1.0f);
	}

	static float CopySign(float a, float b)
	{
		return fabs(a) * Sign(b);
	}

	static int32_t FloatToInt(const float& f, uint8_t precision)
	{
		return (int32_t) (f * pow(10, precision));
	}

	template<typename T>
	static const T& Clamp(const T& x, const T& min, const T& max)
	{
		if (x < min)
			return min;

		if (x > max)
			return max;

		return x;
	}
};
}

#endif