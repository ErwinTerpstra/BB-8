#ifndef _MOTORS_H_
#define _MOTORS_H_

#include "bb8.h"

namespace bb8
{
	
	struct Motor
	{
		uint8_t pinPWM;
		uint8_t pinForward;
		uint8_t pinReverse;
		bool invert;

		Vector2 direction;
	};

	void InitMotors();
	void UpdateMotors();

	void SetDirection(const Vector2& direction);

}

#endif