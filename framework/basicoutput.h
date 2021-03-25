#pragma once
#include "Arduino.h"
#include "timer.h"
#include "ownpin.h"
#include "slotable.h"

class BasicOutput : public OwnPin, public Slotable
{
	Timer switchTimer;
	bool inverted = false;
	unsigned long int mainBlinkTime = 300;
	int blinkCounter = 0;
	void offBlink();
public:
	BasicOutput(int pinid, bool invertLogic = false, int setBlinkTimeInMS = 300);
	void on();
	void off();
	void blink();
	void blinkMulti(int count, int speed=-1);
	void switchState();
	bool isBusy();
};
#ifdef ARDUINO_IDE 
#include "basicoutput.cpp" 
#endif
