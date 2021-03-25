#include "trigger.h"

bool Trigger::isPushed()
{
	return !OwnPin::getState();
}

Trigger::Trigger(int id):
	Process("trigger", GlobalWorkerHandler::Priority::RealTime), 
	OwnPin(id, OwnPin::PinType::INPUT_PULLUP_PIN) {}

uint8_t Trigger::init()
{
	return 0;
}

void Trigger::work()
{
	bool state = !OwnPin::getState();
	if (state) {
		if(counter< counterMax) counter++;
		if (counter >= counterMax/2 && currentState!= State::PUSHED) {
			stateChanged.emit(State::PUSHED);
			currentState = State::PUSHED;
		}
	}
	else {
		if (counter > 0) {
			counter--;
			if (counter <= 0) {
				if(currentState!=State::RELEASED) {
					stateChanged.emit(State::RELEASED);
					released.emit();
					currentState = State::RELEASED;
				}
				counter = 0;
			}
		}
	}
}
