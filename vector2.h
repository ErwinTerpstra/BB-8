#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include "debug.h"

namespace bb8
{


struct Vector2
{
	// Union that allows use of vector type as euler rotation container
	union
	{
		struct { float x, y; };
		struct { float axes[2]; };
	};

	Vector2() : x(0.0f), y(0.0f)
	{

	}

	Vector2(float x, float y) : x(x), y(y)
	{

	}

	void Normalize()
	{
		float length = Length();

		assert(length > 0.0f && "Zero vector can't be normalized!");

		float recipLength = 1.0f / length;
		x *= recipLength;
		y *= recipLength;
	}

	Vector2 Normalized()
	{
		Vector2 result = *this;
		result.Normalize();

		return result;
	}

	Vector2 ComponentMultiply(const Vector2& rhs)
	{
		Vector2 result = *this;
		result.x *= rhs.x;
		result.y *= rhs.y;

		return result;
	}

	__inline float LengthSq() const { return x * x + y * y; }
	float Length() const { return sqrt(LengthSq()); }

	const float& operator[](const int index) const
	{
		return axes[index];
	}
	
	float& operator[](const int index)
	{
		return axes[index];
	}

	Vector2& operator=(const Vector2& other)
	{
	    if (this == &other)
	    	return *this;

	    x = other.x;
	    y = other.y;

	    return *this;
	}

	Vector2& operator +=(const Vector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	Vector2& operator -=(const Vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	Vector2& operator *=(const float& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}
	
	Vector2 operator-() const
	{
		return Vector2(-x, -y);
	}

	bool IsValid() const
	{
		return !(isnan(x) || isnan(y));
	}

	void Print(uint8_t precision = 2) const
	{
		char format[64];
		sprintf(format, "%%.%df;%%.%df;\n", precision, precision);
		Debug::Print(format, x, y);
	}

	static float Angle(const Vector2& a, const Vector2& b)
	{
		return acos(Dot(a, b));
	}

	static float Dot(const Vector2& a, const Vector2& b)
	{
		return a.x * b.x + a.y * b.y;
	}
	
	static Vector2 Zero() { return Vector2(); }
};
	
// Binary operators
__inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
	Vector2 result = lhs;
	return result += rhs;
}

__inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
	Vector2 result = lhs;
	return result -= rhs;
}

__inline Vector2 operator*(const Vector2& lhs, const float& rhs)
{
	Vector2 result = lhs;
	return result *= rhs;
}

// Inversed binary operators
__inline Vector2 operator*(const float& lhs, const Vector2& rhs)
{
	return rhs * lhs;
}


}

#endif