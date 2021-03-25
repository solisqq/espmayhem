#pragma once
#include "Arduino.h"
#include "Wire.h"

class I2CScanner: public Process {
	uint8_t sda;
	uint8_t scl;
public:
	I2CScanner(uint8_t sda, uint8_t scl):Process("scanner",GlobalWorkerHandler::Priority::Normal), sda(sda), scl(scl){}
	uint8_t init() override {
		Wire.begin(sda,scl);
        
	}
	void work() {
        byte error, address;
        int nDevices;

        Serial.println("Scanning...");

        nDevices = 0;
        for (address = 1; address < 127; address++)
        {
            // The i2c_scanner uses the return value of
            // the Write.endTransmisstion to see if
            // a device did acknowledge to the address.
            Wire.beginTransmission(address);
            error = Wire.endTransmission();

            if (error == 0)
            {
                Serial.print("I2C device found at address 0x");
                if (address < 16)
                    Serial.print("0");
                Serial.print(address, HEX);
                Serial.println("  !");

                nDevices++;
            }
            else if (error == 4)
            {
                Serial.print("Unknown error at address 0x");
                if (address < 16)
                    Serial.print("0");
                Serial.println(address, HEX);
            }
        }
        if (nDevices == 0)
            Serial.println("No I2C devices found\n");
        else
            Serial.println("done\n");
	}
};

#ifdef ARDUINO_IDE 
#include "i2cscanner.cpp" 
#endif
