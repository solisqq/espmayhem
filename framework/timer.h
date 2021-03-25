#pragma once
#include "signal.h"
#include "list.h"
#include "worker.h"
#include "GlobalWorkerHandler.h"
#include "nlimits.h"

class Timer : protected Worker
{
	unsigned long activateTime{};
	unsigned long prevTime{};
	unsigned long (*timeFunct)();
	GlobalWorkerHandler::Priority prio;
	bool working = false;
public:
	void start();
	void restart();
	void start(unsigned long time);
	void setTime(unsigned long time);
	void stop();
	Signal<void> timeout;
	Timer(unsigned long (*timeFunction)(), unsigned long time=limits<unsigned long>::max(), const GlobalWorkerHandler::Priority& priority = GlobalWorkerHandler::Priority::Normal);
	Timer(unsigned long (*timeFunction)(), const GlobalWorkerHandler::Priority& priority = GlobalWorkerHandler::Priority::Normal);
	~Timer();
	bool isWorking();
private:
	void work() override;
};

#ifdef ARDUINO_IDE 
#include "timer.cpp" 
#endif


