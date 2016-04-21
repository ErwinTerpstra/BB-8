#ifndef _PID_H_
#define _PID_H_

#include "Arduino.h"

#include "debug.h"

namespace bb8
{
	
	struct PidController
	{
		// PID coëfficients
		const int32_t kp;
		const int32_t ki;
		const int32_t kd;

		int32_t integratedError;

		int32_t target;
		int32_t output;
		int32_t lastError;

		PidController(int32_t kp, int32_t ki, int32_t kd) : 
			kp(kp), ki(ki), kd(kd), 
			integratedError(0), target(0), output(0), lastError(0)
		{

		}

		void Update(int32_t input)
		{
			int32_t error = target - input;
			integratedError += error;

			int32_t deltaError = error - lastError;
			output = kp * error + ki * integratedError + kd * deltaError;

			lastError = error;
		}

		void Reset()
		{
			integratedError = 0;

			target = 0;
			output = 0;
			lastError = 0;
		}
		

		void Debug()
		{
			Debug::Print("Output: %ld; Error: %ld; Integral: %ld;\n", output, lastError, integratedError);
		}
	};
}

#endif