#include "basicoutput.h"

BasicOutput::BasicOutput(int pinid, bool invertLogic, int setBlinkTimeInMS): 
	OwnPin(pinid, OwnPin::PinType::OUTPUT_PIN, invertLogic?HIGH:LOW),
	inverted(invertLogic),
	switchTimer(millis, setBlinkTimeInMS, GlobalWorkerHandler::Priority::Normal),
	mainBlinkTime(setBlinkTimeInMS)
{
	off();
	switchTimer.timeout.attach(this, &BasicOutput::offBlink);
}

void BasicOutput::on()
{
	setState(!inverted);
}

void BasicOutput::blink() {
	on();
	blinkCounter = 1;
	switchTimer.start();
}

void BasicOutput::blinkMulti(int count, int speed)
{
	on();
	blinkCounter = count;
	if(speed>0) switchTimer.setTime(speed);
	switchTimer.start();
}

void BasicOutput::off() {
	setState(inverted);
}

void BasicOutput::offBlink() {
	if (getState()==!inverted) {
		blinkCounter--;
		off();
		if (blinkCounter <= 0) {
			switchTimer.setTime(mainBlinkTime);
			switchTimer.stop();
			blinkCounter = 0;
			return;
		}
	}
	else if (blinkCounter > 0) on();
}

void BasicOutput::switchState()
{
	setState(!getState());
}

bool BasicOutput::isBusy() {return blinkCounter > 0;}
