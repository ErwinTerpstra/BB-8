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

		float currentPower;

		void SetAngle(float angle)
		{
			direction.x = cos(angle * DEG_2_RAD);
			direction.y = sin(angle * DEG_2_RAD);
		}
	};

	void InitMotors();
	void UpdateMotors();
	void PrintMotorData();

	void SetDirection(const Vector2& direction);

}

#endif