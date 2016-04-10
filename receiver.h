#ifndef _RECEIVER_H_
#define _RECEIVER_H_

namespace bb8
{

	enum Channels
	{
		THROTTLE,
		ELEVATOR,
		AILERON,
		RUDDER,

		AUX1,
		AUX2,

		NUM_CHANNELS,
	};

	struct ReceiverData
	{
		uint16_t channels[NUM_CHANNELS];

		void SetChannel(Channels channel, uint16_t pulse);
		float GetNormalizedChannel(Channels channel);
	};


	void InitReceiver();
	void UpdateReceiver();
	void PrintChannels();
	
	ReceiverData& GetReceiverData();

}

#endif