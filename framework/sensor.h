#pragma once

template <typename T>
class Sensor //: public HProcess
{
	T value;
	virtual void init() = 0;
	virtual void work() = 0;
};

#ifdef ARDUINO_IDE 
#include "sensor.cpp" 
#endif