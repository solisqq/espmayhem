#pragma once
#include "slot.h"
#include "list.h"
#include "slotable.h"

template<typename RetType, typename ... Args>
class Signal : protected SignalInterface
{
	List<SlotInterface<Args...>*> memberSlots;
	List<RetType(*)(Args...)> nonMemberSlots;
public:
	~Signal() {
		for (auto slot : memberSlots) delete slot;
	}
	template<typename Type>
	void attach(Type* object, RetType(Type::* m)(Args...)) {
		static_cast<Slotable*>(object)->appendSignal(this);
		memberSlots.pushBack(new Slot<Type, RetType, Args...>(object, m));
	}
	void attach(RetType(*m)(Args...)) {
		nonMemberSlots.pushBack(m);
	}
	void emit(Args... args) {
		for (auto slot : memberSlots) slot->call(args...);
		for (auto slot : nonMemberSlots) slot(args...);
	}
private:
	void removeSlotsConnectedWithTarget(Slotable* ptr) override {
		for (auto &slotInterface : memberSlots) {
			if (slotInterface->isSame(ptr)) {
				if (!memberSlots.removeAll(slotInterface)) Serial.println("Nie znalazlem");
			}
		}
	}
};

#ifdef ARDUINO_IDE 
#include "signal.cpp" 
#endif


/*
template<typename Arg, typename RetType = void>
class Signal
{
	List<SlotInterface<Arg>*> memberSlots;
	List<RetType(*)(Arg)> nonMemberSlots;
public:
	~Signal();
	template<typename Type>
	void attach(Type* object, RetType(Type::* m)(Arg));
	void attach(RetType(*m)(Arg));
	void emit(const Arg& arg);
};


template<typename Arg, typename RetType>
inline Signal<Arg, RetType>::~Signal() {
	for (auto slot : memberSlots) {
		delete slot;
	}
}

template<typename Arg, typename RetType>
inline void Signal<Arg, RetType>::attach(RetType(*m)(Arg)) {
	nonMemberSlots.pushBack(m);
}

template<typename Arg, typename RetType>
inline void Signal<Arg, RetType>::emit(const Arg& arg) {
	for (auto slot : memberSlots) {
		slot->call(arg);
	}
	for (auto slot : nonMemberSlots) {
		slot(arg);
	}
}

template<typename Arg, typename RetType>
template<typename Type>
inline void Signal<Arg, RetType>::attach(Type* object, RetType(Type::* m)(Arg)) {
	memberSlots.pushBack(new Slot<Type, Arg, RetType>(object, m));
}*/
