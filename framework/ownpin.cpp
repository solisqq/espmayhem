#include "ownpin.h"

OwnPin::OwnPin(int id, const PinType& type, int defaultState):pin(id)
{
	pinMode(id, static_cast<int>(type));
	if (defaultState != -1) setState(defaultState);
}

void OwnPin::setPin(int id)
{
	pin = id;
}

bool OwnPin::getState()
{
	return static_cast<bool>(digitalRead(pin));
}

void OwnPin::setState(bool state)
{
	digitalWrite(pin, static_cast<int>(state));
}