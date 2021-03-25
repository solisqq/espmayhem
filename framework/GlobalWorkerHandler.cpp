#include "GlobalWorkerHandler.h"

inline void GlobalWorkerHandler::work() {
	doRT();
	doNormal();
}

void GlobalWorkerHandler::addWorker(Worker& worker, const Priority& prio)
{
	if (prio == Priority::Normal) normal_timers.pushBack(&worker);
	else rt_timers.pushBack(&worker);
}

void GlobalWorkerHandler::removeWorker(Worker& toRemove)
{
	rt_timers.removeAll(&toRemove);
	normal_timers.removeAll(&toRemove);
}

inline void GlobalWorkerHandler::doNormal() {
	int i = 0;
	for (auto normal : normal_timers) {
		i++;
		if (i % 3 == 0) doRT();
		normal->work();
	}
}

inline void GlobalWorkerHandler::doRT() {
	for (auto rt : rt_timers) rt->work();
}

GlobalWorkerHandler globalWorkerHandler;