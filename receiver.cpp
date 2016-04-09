#include <CPPM.h>

#include "bb8.h"
#include "receiver.h"

using namespace bb8;


static ReceiverData receiverData;

void bb8::InitReceiver()
{
	CPPM.begin();

	receiverData.SetChannel(THROTTLE, 	1000);
	receiverData.SetChannel(AILERON, 	1500);
	receiverData.SetChannel(ELEVATOR, 	1500);
	receiverData.SetChannel(RUDDER, 	1500);
}

void bb8::UpdateReceiver()
{
	CPPM.cycle();

	if (CPPM.synchronized())
	{
		receiverData.SetChannel(THROTTLE, 	(uint16_t) CPPM.read_us(CPPM_THRO));
		receiverData.SetChannel(AILERON, 	(uint16_t) CPPM.read_us(CPPM_AILE));
		receiverData.SetChannel(ELEVATOR, 	(uint16_t) CPPM.read_us(CPPM_ELEV));
		receiverData.SetChannel(RUDDER, 	(uint16_t) CPPM.read_us(CPPM_RUDD));
	}
}

ReceiverData& bb8::GetReceiverData()
{
	return receiverData;
}

void ReceiverData::SetChannel(Channels channel, uint16_t pulse)
{
	this->channels[(uint8_t) channel] = pulse;
}

float ReceiverData::GetNormalizedChannel(Channels channel)
{
	uint16_t pulseLength = this->channels[(uint8_t) channel];
	return (pulseLength - 1500.0f) / 500.0f;
}