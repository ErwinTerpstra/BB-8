#include <Wire.h>

#include "i2c.h"

#include "imu.h"
#include "receiver.h"
#include "motors.h"

#include "pid.h"

using namespace bb8;

PidController pidPitch(STABILIZE_KP, STABILIZE_KI, STABILIZE_KD);
PidController pidRoll(STABILIZE_KP, STABILIZE_KI, STABILIZE_KD);

uint32_t debugTimer = 0;
uint32_t stabilizeTimer = 0;

uint32_t frameTime = 0;

void setup()
{
	Serial.begin(115200);

	I2C::Setup();

	imu::InitIMU();
	receiver::InitReceiver();
	actuators::InitMotors();

	frameTime = micros();

	stabilizeTimer = frameTime;
	debugTimer = frameTime;
}

void loop()
{
	uint32_t time = micros();
	uint32_t dt = time - frameTime;
	frameTime = time;

	receiver::UpdateReceiver();
	actuators::UpdateMotors(dt);
	
	receiver::ReceiverData& receiverData = receiver::GetReceiverData();
	Vector2 direction(-receiverData.GetNormalizedChannel(receiver::AILERON), receiverData.GetNormalizedChannel(receiver::ELEVATOR));

	bool stabilize = receiverData.GetNormalizedChannel(receiver::AUX1) > 0.0f;

	if (stabilize)
	{
		if (time - stabilizeTimer > STABILIZE_UPDATE_INTERVAL)
		{
			imu::UpdateIMU(STABILIZE_UPDATE_INTERVAL);
			const Quaternion& orientation = imu::GetCurrentOrientation();

			float yaw, pitch, roll;
			orientation.ToEulerAngles(yaw, pitch, roll);

			pidPitch.Update((int32_t) (pitch * STABILIZE_PRECISION));
			pidRoll.Update((int32_t) (roll * STABILIZE_PRECISION));

			stabilizeTimer += STABILIZE_UPDATE_INTERVAL;
		}

		direction.x += pidRoll.output / (float) (STABILIZE_PRECISION * STABILIZE_OUTPUT_SCALE);
		direction.y += pidPitch.output / (float) (STABILIZE_PRECISION * STABILIZE_OUTPUT_SCALE);
	}
	else
	{
		pidPitch.Reset();
		pidRoll.Reset();
	}

	if (direction.LengthSq() > 1.0)
		direction.Normalize();

	actuators::SetDirection(direction);
	actuators::SetHeadAngle(-receiverData.GetNormalizedChannel(receiver::RUDDER));

	if (time - debugTimer > DEBUG_INTERVAL)
	{
		//actuators::PrintMotorData();

		pidPitch.Debug();

		debugTimer = time;
	}
}