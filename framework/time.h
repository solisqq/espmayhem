#pragma once
class utime
{
	enum class Format : unsigned long {
		Seconds=1,
		Milliseconds=1000,
		Microseconds=1000000
	};
};
#ifdef ARDUINO_IDE 
#include "time.cpp" 
#endif
