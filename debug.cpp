#include "Arduino.h"

#include "debug.h"

using namespace bb8;

Debug::Debug()
{
	
}

void Debug::Print(const char* msg, va_list args)
{
	vsprintf(printBuffer, msg, args);

	Serial.print(printBuffer);
}

void Debug::Print(const __FlashStringHelper* msg, va_list args)
{
	const char PROGMEM *p = (const char PROGMEM *)msg;

	uint16_t d;
  	for (d = 0; d < FLASH_BUFFER_SIZE - 1; ++d)
  	{
    	unsigned char c = pgm_read_byte(p + d);

    	if (c == 0)
    		break;

    	flashBuffer[d] = c;
  	}

  	flashBuffer[d] = 0;

	Print(flashBuffer, args);
}

void Debug::Print(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	Instance().Print(msg, args);
	va_end(args);
}

void Debug::Print(const __FlashStringHelper* msg, ...)
{
	va_list args;
	va_start(args, msg);


	Instance().Print(msg, args);

	va_end(args);
}

bool Debug::AssertHandler(const char* code, const char* file, const uint32_t line)
{
	Print(F("Assert failed!\n%s at %s:%d\n"), code, file, line);
	return true;
}

bool Debug::Halt()
{
	#ifdef BB_DEBUG
		while(1);
	#endif

	return true;
}