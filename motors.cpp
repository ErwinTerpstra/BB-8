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
		
		motor.SetAngle(90.0f);
	}
	
	{
		// LEFT-BACK
		Motor& motor = motors[1];
		motor.pinPWM			= 5;
		motor.pinForward		= 15;
		motor.pinReverse		= 14;
		motor.invert 			= false;

		motor.SetAngle(210.0f);
	}

	{
		// RIGHT-BACK
		Motor& motor = motors[2];
		motor.pinPWM			= 11;
		motor.pinForward		= 4;
		motor.pinReverse		= 3;
		motor.invert 			= true;
		
		motor.SetAngle(330.0f);
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

void bb8::PrintMotorData()
{
	Debug::Print("%ld; %ld; %ld\n", 
		Util::FloatToInt(motors[0].currentPower, 2), 
		Util::FloatToInt(motors[1].currentPower, 2),
		Util::FloatToInt(motors[2].currentPower, 2));
}

void bb8::SetDirection(const Vector2& direction)
{
	for (uint8_t motorIdx = 0; motorIdx < NUM_MOTORS; ++motorIdx)
	{
		Motor& motor = motors[motorIdx];

		motor.currentPower = Vector2::Dot(motor.direction, direction);
	}

	for (uint8_t motorIdx = 0; motorIdx < NUM_MOTORS; ++motorIdx)
	{
		Motor& motor = motors[motorIdx];

		uint8_t forward = motor.currentPower > 0.0f;

		if (motor.invert)
			forward = 1 - forward;

		digitalWrite(motor.pinForward, forward);
		digitalWrite(motor.pinReverse, 1 - forward);

		uint8_t pwm = (uint8_t) (fabs(motor.currentPower) * 255);
		analogWrite(motor.pinPWM, pwm);
	}

}