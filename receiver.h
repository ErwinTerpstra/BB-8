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
	
	ReceiverData& GetReceiverData();

}

#endif