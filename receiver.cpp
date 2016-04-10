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
	receiverData.SetChannel(AUX1, 		1000);
	receiverData.SetChannel(AUX2, 		1000);
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
		receiverData.SetChannel(AUX1, 		(uint16_t) CPPM.read_us(CPPM_GEAR));
		receiverData.SetChannel(AUX2, 		(uint16_t) CPPM.read_us(CPPM_AUX1));
	}
}

void bb8::PrintChannels()
{
	Debug::Print("Channels: \n");
	Debug::Print("%ld; %ld; %ld; %ld; %ld; %ld\n", 
		Util::FloatToInt(receiverData.GetNormalizedChannel(THROTTLE), 4), Util::FloatToInt(receiverData.GetNormalizedChannel(AILERON), 4),
		Util::FloatToInt(receiverData.GetNormalizedChannel(ELEVATOR), 4), Util::FloatToInt(receiverData.GetNormalizedChannel(RUDDER), 4),
		Util::FloatToInt(receiverData.GetNormalizedChannel(AUX1), 4), Util::FloatToInt(receiverData.GetNormalizedChannel(AUX2), 4));
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

	if (pulseLength < RC_MID_SIGNAL)
		return -((RC_MID_SIGNAL - pulseLength) / (float) (RC_MID_SIGNAL - RC_MIN_SIGNAL));
	else
		return (pulseLength - RC_MID_SIGNAL) / (float) (RC_MAX_SIGNAL - RC_MID_SIGNAL);
}