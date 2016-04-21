#ifndef _IMU_H_
#define _IMU_H_

#include "bb8.h"

namespace bb8
{

namespace imu
{

	void InitIMU();
	void UpdateIMU(uint32_t dt);
	void PrintOrientation();

	void Calibrate();

	void SetupMPU();
	void ReadMPU();

	void ReadAcceleration(Vector3& acceleration);
	void ReadGyro(Vector3& angularVelocity);

	__inline void ConvertVector(Vector3& v)
	{
		float tmp = v.y;

		v.y = -v.z;
		v.z = tmp;
	}

	const Quaternion& GetCurrentOrientation();
	
}

}

#endif