#include "imu.h"

#include "i2c.h"

#include "filter.h"
#include "mpu6050.h"

using namespace bb8;
using namespace bb8::imu;


MPU6050Data mpuData;

uint16_t accelRange = 0, gyroRange = 0;
float accelScale = 1.0f, gyroScale = 1.0f;

Vector3 gyroOffset;

Quaternion orientation;
Vector3 angularVelocity, acceleration;

//Filter<Vector3> accelerationFilter(Filter<Vector3>::HIGH_PASS, ACCEL_FILTER_RC);
//Filter<Vector3> angularVelocityFilter(Filter<Vector3>::LOW_PASS, GYRO_FILTER_RC);

void bb8::imu::InitIMU()
{
	SetupMPU();
	Calibrate();
}

void bb8::imu::UpdateIMU(uint32_t dt)
{
	// Read new values from i2c
	ReadMPU();

	//ReadGyro(angularVelocity);
	ReadAcceleration(acceleration);

	//angularVelocity = angularVelocityFilter.Sample(angularVelocity, deltaSeconds);
	//acceleration = accelerationFilter.Sample(acceleration, deltaSeconds);
	
	// Only use accelerometer values if total acceleration is below threshold
	float magnitude = acceleration.Length();
	if (fabs(1.0f - magnitude) < ACCEL_MAX)
	{
		Vector3 up = -acceleration;

		up.Normalize();
		Quaternion::RotationBetween(orientation, up, Vector3::Up());
	}
}


void bb8::imu::PrintOrientation()
{
	float yaw, pitch, roll;

	orientation.ToEulerAngles(yaw, pitch, roll);
	Debug::Print(F("Orientation:\n"));
	Debug::Print(F("Yaw = %d; Pitch = %d; Roll = %d\n"), 
		(int16_t) yaw, (int16_t) pitch, (int16_t) roll);

	Debug::Print(F("\n"));

	Debug::Print(F("Acceleration: %ld;%ld;%ld;\tLength:%ld\n"), 
		Util::FloatToInt(acceleration.x, 4), Util::FloatToInt(acceleration.y, 4), Util::FloatToInt(acceleration.z, 4), 
		Util::FloatToInt(acceleration.Length(), 4));

	/*
	Debug::Print("Ang. vel.: %ld;%ld;%ld;\tLength:%ld\n", 
		Util::FloatToInt(angularVelocity.x, 4), Util::FloatToInt(angularVelocity.y, 4), Util::FloatToInt(angularVelocity.z, 4), 
		Util::FloatToInt(angularVelocity.Length(), 4));
		*/
}


void bb8::imu::Calibrate()
{
	Debug::Print(F("Calibrating accelerometer with %d samples\n"), CALIBRATION_SAMPLES);

	float scale = 0.0f;
	Vector3 averageGyro = Vector3::Zero();

	// TODO: calibrate initial orientation from accelerometer

	// Read multiple samples to find real accelerometer scale
	for (uint8_t sampleIdx = 0; sampleIdx < CALIBRATION_SAMPLES; ++sampleIdx)
	{
		// Read new values from i2c
		ReadMPU();

		ReadAcceleration(acceleration);
		ReadGyro(angularVelocity);

		// The current length of the acceleration vector is the error in scale
		scale += acceleration.Length() / CALIBRATION_SAMPLES;

		// Angular velocity in rest is the offset the gyro has
		averageGyro += angularVelocity * (1.0f / CALIBRATION_SAMPLES);

		// Wait a while until next sample
		delay(CALIBRATION_INTERVAL);
	}

	Debug::Print(F("Accelerometer calibrated at %ld\n"), Util::FloatToInt(scale, 4));
	Debug::Print(F("Gyroscope offset calibrated at: %ld;%ld;%ld\n"), 
		Util::FloatToInt(averageGyro.x, 4), Util::FloatToInt(averageGyro.y, 4), Util::FloatToInt(averageGyro.z, 4));

	// Apply calibration scale to the conversion scale
	accelScale *= 1.0f / scale;
	gyroOffset = averageGyro;
}

void bb8::imu::SetupMPU()
{
	// Default at MPU6050 settings at power-up:
	//    Gyro at 250 degrees second
	//    Acceleration at 2g
	//    Clock source at internal 8MHz
	//    The device is in sleep mode.
	//

	// TODO: generalize this to the config or a constant
	// Set the accelerometer range to 4G and the gyro range to 500 degrees/s
	accelRange = 4;
	accelScale = (1.0f / INT16_MAX) * accelRange;

	gyroRange = 500; 
	gyroScale = (1.0f / INT16_MAX) * gyroRange;
	I2C::WriteRegister(MPU6050_I2C_ADDRESS, MPU6050_ACCEL_CONFIG, MPU6050_AFS_SEL_4G);
	I2C::WriteRegister(MPU6050_I2C_ADDRESS, MPU6050_GYRO_CONFIG, MPU6050_FS_SEL_500);

	// Clear the 'sleep' register of the MPU6050 to start recording data
	I2C::WriteRegister(MPU6050_I2C_ADDRESS, MPU6050_PWR_MGMT_1, 0);
}

void bb8::imu::ReadMPU()
{
	uint8_t error = I2C::Read(MPU6050_I2C_ADDRESS, MPU6050_ACCEL_XOUT_H, (uint8_t*) &mpuData, sizeof(mpuData));

	if (error != I2C::ERR_OK)
		Debug::Print(F("I2C error while reading MPU: %ld\n"), error);

	// Correct endianness difference between uC and mpu data
	Util::SwapEndianness((uint8_t*) &mpuData.accelX, sizeof(mpuData.accelX));
	Util::SwapEndianness((uint8_t*) &mpuData.accelY, sizeof(mpuData.accelY));
	Util::SwapEndianness((uint8_t*) &mpuData.accelZ, sizeof(mpuData.accelZ));

	Util::SwapEndianness((uint8_t*) &mpuData.temperature, sizeof(mpuData.temperature));

	Util::SwapEndianness((uint8_t*) &mpuData.gyroX, sizeof(mpuData.gyroX));
	Util::SwapEndianness((uint8_t*) &mpuData.gyroY, sizeof(mpuData.gyroY));
	Util::SwapEndianness((uint8_t*) &mpuData.gyroZ, sizeof(mpuData.gyroZ));
}

void bb8::imu::ReadAcceleration(Vector3& acceleration)
{
	// Convert accelerometer values to G normalized
	acceleration.x = mpuData.accelX;
	acceleration.y = mpuData.accelY;
	acceleration.z = mpuData.accelZ;

	acceleration *= accelScale;

	ConvertVector(acceleration);
}

void bb8::imu::ReadGyro(Vector3& angularVelocity)
{
	angularVelocity.x = mpuData.gyroX;
	angularVelocity.y = mpuData.gyroY;
	angularVelocity.z = mpuData.gyroZ;

	angularVelocity *= gyroScale * DEG_2_RAD;
	angularVelocity -= gyroOffset;
	
	ConvertVector(angularVelocity);
}

const Quaternion& bb8::imu::GetCurrentOrientation() { return orientation; }