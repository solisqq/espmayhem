#pragma once
#include "worker.h"
#include "list.h"

class GlobalWorkerHandler : public Worker {
	List<Worker*> rt_timers;
	List<Worker*> normal_timers;
public:
	enum class Priority {
		RealTime,
		Normal
	};
	void work() override;
	void addWorker(Worker&, const Priority&);
	void removeWorker(Worker& toRemove);
private:
	void doNormal();
	void doRT();
};
extern GlobalWorkerHandler globalWorkerHandler;

#ifdef ARDUINO_IDE 
#include "GlobalWorkerHandler.cpp" 
#endif
