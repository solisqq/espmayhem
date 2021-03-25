#include "gun.h"
//sda 13, scl 14
Gun::Gun(): 
	network(IPAddress(192,168,0,19),"Undwik", "2/38Haslo*"),
	hclient("192.168.0.87"),
	laser(Global::Pins::LASER, true, 200), 
	buzzer(Global::Pins::BUZZER, false, 200), 
	trigger(Global::Pins::TRIGGER),
	display(13,14)
{
	if (display.init() != 0) goutput.warning("#DISPFAIL");
	else goutput.warning("DISPGOOD");
	trigger.stateChanged.attach(this, &Gun::onTrigger);
	trigger.start(1000);

	if (network.start(1) != 0) {
		goutput.error("#NETINIT");
		Serial.println("test");
	}
	goutput.info("NETSUCC");
	otaserv.start(100);
	goutput.info("OTASUCC");
	if (trigger.isPushed()) {
		goutput.info("PROGWAIT");
		return;
	}
	if (hclient.start(100) != 0) goutput.warning("!CLIENTDOWN");
	else goutput.info("CLIENTOK");
	if (camera.init() != 0) goutput.warning("!CAMFAIL");
	else goutput.info("CAMSUCC");

	hclient.connectionStateChanged.attach([](bool state) {
		if (state) goutput.info("CONSUCC");
		else goutput.warning("!CONFAIL"); });
	goutput.info("INITSUCC");
}

BasicOutput& Gun::getLaser()
{
	return laser;
}

BasicOutput& Gun::getBuzzer()
{
	return laser;
}

void Gun::onTrigger(Trigger::State state)
{
	if (state == Trigger::State::PUSHED && hclient.isConnected()) {
		if (!hclient.isAvailableForIMG()) {
			buzzer.blinkMulti(3, 100);
			return;
		}
		auto img = camera.getPicture();
		if (!img) {
			goutput.warning("!CAMFAIL");
			buzzer.blinkMulti(3, 100);
			return;
		}
		buzzer.blink();
		laser.blink();
		hclient.sendIMG(img);
		camera.releaseBuffer(img);
	}
}
