#include "timer.h"

void Timer::start()
{
	prevTime = timeFunct();
	globalWorkerHandler.addWorker(*this, prio);
	working = true;
}

void Timer::start(unsigned long time){setTime(time);start();}
void Timer::setTime(unsigned long time){activateTime = time;}

void Timer::stop() {
	globalWorkerHandler.removeWorker(*this);
	working = false;
}

void Timer::restart() {
	if (!working) start();
	else prevTime = timeFunct();
}

Timer::Timer(unsigned long (*timeFunction)(), unsigned long time, const GlobalWorkerHandler::Priority& priority) :
	activateTime(time),
	timeFunct(timeFunction),
	prio(priority){
	start();
}

Timer::Timer(unsigned long (*timeFunction)(), const GlobalWorkerHandler::Priority& priority):
	prio(priority), 
	timeFunct(timeFunction),
	activateTime(limits<unsigned long>::max()) {}

Timer::~Timer(){stop();}

bool Timer::isWorking()
{
	return working;
}

void Timer::work() {
	unsigned long currentTime = timeFunct();
	if (prevTime + activateTime <= currentTime) {
		prevTime = currentTime;
		timeout.emit();
	}
}