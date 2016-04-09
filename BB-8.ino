#include <Wire.h>

#include "i2c.h"

#include "receiver.h"
#include "motors.h"

using namespace bb8;

uint32_t debugTimer = 0;

void setup()
{
	Serial.begin(9600);

	I2C::Setup();

	InitReceiver();
	InitMotors();
}

void loop()
{
	UpdateReceiver();
	UpdateMotors();

	uint32_t time = micros();

	if (time - debugTimer > 500)
	{
		ReceiverData& receiverData = GetReceiverData();
		Vector2 direction(-receiverData.GetNormalizedChannel(AILERON), receiverData.GetNormalizedChannel(ELEVATOR));

		if (direction.LengthSq() > 1.0)
			direction.Normalize();

		SetDirection(direction);

		Debug::Print("%d;%d\n", (int) (direction.x * 100), (int) (direction.y * 100));

		debugTimer = time;
	}
}