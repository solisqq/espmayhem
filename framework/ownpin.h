#pragma once
#include "Arduino.h"

class OwnPin
{
	int pin = 100;
public:
	enum class PinType: int {
		OUTPUT_PIN=2,
		INPUT_PIN=1,
		INPUT_PULLUP_PIN=5
	};
	OwnPin(int, const PinType&, int=-1);
	void setPin(int id);
	bool getState();
	void setState(bool state);
	void On();
	void Off();
};
#ifdef ARDUINO_IDE 
#include "ownpin.cpp" 
#endif
