#pragma once
#include "list.h"

class Slotable;
class SignalInterface {
public:
	virtual void removeSlotsConnectedWithTarget(Slotable* ptr) {};
};
class Slotable {
	List<SignalInterface*> signals;
public:
	void appendSignal(SignalInterface* signal) { signals.pushBack(signal); };
	virtual ~Slotable() {
		for (auto signal : signals) signal->removeSlotsConnectedWithTarget(this);
	}
};
