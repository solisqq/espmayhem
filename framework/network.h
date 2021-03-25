#pragma once
#include "Arduino.h"
#include "hprocess.h"
#include "WiFi.h"
#include "credentials.h"
#include "maintain.h"

class Network: public Process, private Credentials {
	
	uint8_t init() override;
public:
	Network(const IPAddress& addr, const String& ssid, const String& pass);
	void work() override;
};

#ifdef ARDUINO_IDE 
#include "network.cpp" 
#endif
