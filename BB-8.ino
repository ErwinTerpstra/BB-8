#include <Wire.h>

#include "i2c.h"

#include "imu.h"
#include "receiver.h"
#include "motors.h"

using namespace bb8;

uint32_t debugTimer = 0;
uint32_t frameTime = 0;

void setup()
{
	Serial.begin(115200);

	I2C::Setup();

	InitIMU();
	InitReceiver();
	InitMotors();

	frameTime = micros();
}

void loop()
{
	uint32_t time = micros();
	uint32_t dt = time - frameTime;

	UpdateIMU(dt);

	UpdateReceiver();
	UpdateMotors();
	
	ReceiverData& receiverData = GetReceiverData();
	Vector2 direction(-receiverData.GetNormalizedChannel(AILERON), receiverData.GetNormalizedChannel(ELEVATOR));

	bool stabilize = receiverData.GetNormalizedChannel(AUX1) > 0.0f;

	if (stabilize)
	{
		const Quaternion& orientation = GetCurrentOrientation();

		float yaw, pitch, roll;
		orientation.ToEulerAngles(yaw, pitch, roll);

		direction.x += (roll / 90.0f) * STABILIZE_FACTOR;
		direction.y += (pitch / 90.0f) * STABILIZE_FACTOR;
	}

	if (direction.LengthSq() > 1.0)
		direction.Normalize();

	SetDirection(direction);

	if (time - debugTimer > 500000)
	{
		PrintMotorData();

		debugTimer = time;
	}
}