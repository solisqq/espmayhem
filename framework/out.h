#pragma once
#include "WString.h"
#include "HardwareSerial.h"
#include "Esp.h"
class Out
{
public:
	Out();
	enum class Type {
		Info,
		Debug,
		Warning,
		Error
	};
	void show(const Type& type, const String& toShow);
private:
	String typeToString(const Type& type);
};

extern Out out;
#ifdef ARDUINO_IDE 
#include "out.cpp" 
#endif
