#pragma once
#include "hprocess.h"

class Camera: public Process {
	Camera();
};
#ifdef ARDUINO_IDE 
#include "trigger.cpp" 
#endif
