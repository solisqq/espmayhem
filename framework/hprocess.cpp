#include "hprocess.h"

uint8_t Process::start(uint16_t frequency)
{
	timer.start(1000000/frequency);
	return init();
}

Process::Process(const String& processName, const GlobalWorkerHandler::Priority& prio):
	Nameable(processName), 
	timer(micros,prio)
{
	timer.timeout.attach<Process>(this, &Process::work);
}
