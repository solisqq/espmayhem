#pragma once
#include "Nameable.h"
#include "worker.h"
#include "timer.h"

class Process : public Nameable, public Worker, public Slotable
{
	Timer timer;
public:
	Process(const String&, const GlobalWorkerHandler::Priority& prio = GlobalWorkerHandler::Priority::Normal);
	uint8_t start(uint16_t frequency);
protected:
	virtual uint8_t init()=0;
};

#ifdef ARDUINO_IDE 
#include "hprocess.cpp" 
#endif

