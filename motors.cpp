#include "motors.h"

using namespace bb8;

const uint8_t NUM_MOTORS = 3;
Motor motors[NUM_MOTORS];

void bb8::InitMotors()
{
	{
		// FORWARD
		Motor& motor = motors[0];
		motor.pinPWM			= 6;
		motor.pinForward		= 17;
		motor.pinReverse		= 16;
		motor.invert 			= true;
		motor.direction 		= Vector2(0.000f, 1.000f);
	}
	
	{
		// LEFT-BACK
		Motor& motor = motors[1];
		motor.pinPWM			= 5;
		motor.pinForward		= 15;
		motor.pinReverse		= 14;
		motor.invert 			= false;
		motor.direction 		= Vector2(-0.866f, -0.500f);
	}

	{
		// RIGHT-BACK
		Motor& motor = motors[2];
		motor.pinPWM			= 11;
		motor.pinForward		= 4;
		motor.pinReverse		= 3;
		motor.invert 			= true;
		motor.direction 		= Vector2(0.866f, -0.500f);
	}

	for (uint8_t motorIdx = 0; motorIdx < NUM_MOTORS; ++motorIdx)
	{
		Motor& motor = motors[motorIdx];

		pinMode(motor.pinPWM, OUTPUT);
		pinMode(motor.pinForward, OUTPUT);
		pinMode(motor.pinReverse, OUTPUT);
	}
}

void bb8::UpdateMotors()
{
	
}

void bb8::SetDirection(const Vector2& direction)
{
	for (uint8_t motorIdx = 0; motorIdx < NUM_MOTORS; ++motorIdx)
	{
		Motor& motor = motors[motorIdx];

		float power = Vector2::Dot(motor.direction, direction);
		uint8_t forward = power > 0.0f;

		if (motor.invert)
			forward = 1 - forward;

		digitalWrite(motor.pinForward, forward);
		digitalWrite(motor.pinReverse, 1 - forward);

		uint16_t pwm = (uint16_t) (fabs(power) * 255);
		analogWrite(motor.pinPWM, pwm);

		//Debug::Print("%u: %u\n", motorIdx, pwm);
	}

}