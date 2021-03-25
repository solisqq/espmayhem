//#define NO_CAMERA_B
#define ARDUINO_IDE
#include "Arduino.h"
#include "framework/settings.h"
#include "framework/gun.h"

Gun *gun = nullptr;

void setup() { 
	//HSettings::get().overwriteWithDefault();
	HSettings::get().loadDataFromMEM();
	gun = new Gun();
}
void loop() {globalWorkerHandler.work();}