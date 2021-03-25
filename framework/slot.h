#pragma once

template <typename ... Args>
class SlotInterface {
public:
	virtual ~SlotInterface() {}
	virtual void call(Args... value) = 0;
	virtual bool isSame(void* ptr)=0;
};

template <typename Target, typename RetType, typename... Args>
class Slot : public SlotInterface<Args...>
{
	Target* object;
	RetType(Target::* method)(Args...);
public:
	Slot(Target* object, RetType(Target::* m)(Args...)) : object(object), method(m) {}
	void call(Args... values) override {
		(object->*method)(values...);
	}
	bool isSame(void* ptr) {
		if (ptr == (void*)object && ptr!=nullptr) return true;
		return false;
	}
};

#ifdef ARDUINO_IDE 
#include "slot.cpp" 
#endif

