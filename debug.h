#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "settings.h"

#include "Arduino.h"

namespace bb8
{
	
class Debug
{
public:
	static const uint16_t PRINT_BUFFER_SIZE = 256;
	static const uint16_t FLASH_BUFFER_SIZE = 128;

private:
	char printBuffer[PRINT_BUFFER_SIZE];

	char flashBuffer[FLASH_BUFFER_SIZE];

	Debug();
	
public:
	void Print(const char* msg, va_list args);

	void Print(const __FlashStringHelper* msg, va_list args);

public:

	static void Print(const char* msg, ...);

	static void Print(const __FlashStringHelper* msg, ...);

	static bool AssertHandler(const char* code, const char* file, const uint32_t line);

	static bool Halt();
	
	static Debug& Instance()
	{
		static Debug instance;

		return instance;
	}
};

#ifdef BB_DEBUG
	#define assert(x) ((void)(!(x) && Debug::AssertHandler(#x, __FILE__, __LINE__) && Debug::Halt()))
#else
	#define assert(x) 
#endif

}

#endif