#pragma once
#include "Arduino.h"
#include "settings.h"
#include "maintain.h"
#include "network.h"
#include "OTAService.h"
#include "camera.h"
#include "trigger.h"
#include "hclient.h"
#include "slotable.h"
#include "hdisplay.h"
#include "tools/i2cscanner.h"

class Gun: public Slotable {
	Network network;
	OTAService otaserv;
	HClient hclient;
	Camera camera;
	BasicOutput laser;
	BasicOutput buzzer;
	Trigger trigger;
	HDisplay display;
public:
	Gun();
	BasicOutput& getLaser();
	BasicOutput& getBuzzer();
private:
	void onTrigger(Trigger::State state);
};

#ifdef ARDUINO_IDE 
#include "gun.cpp" 
#endif
