#include "hclient.h"
#include "hutils.h"
#include "settings.h"
HClient::HClient(const String& ip): 
	Process("client", GlobalWorkerHandler::Priority::Normal), 
	serverAddress(ip),
	pingTimer(millis, GlobalWorkerHandler::Priority::Normal)
{
	inputPackets.pushBack(settingsPacket);
	inputPackets.pushBack(setupPacket);
	pingTimer.setTime(Global::Netw::pingInterval);
	pingTimer.timeout.attach(this, &HClient::sendPing);
	client.setTimeout(1);
}

uint8_t HClient::init(){
	client.stop();
	if (!checkConnection()) return 1;
	pingTimer.start();
	return 0;
}

void HClient::sendIMG(camera_fb_t* img) {
	
	auto packet = ImagePacket::create(img);
	if (sendPacket(packet)) availableForIMGSending = false;
}
void HClient::sendResponse(HInputPacket& packet) {
	for (int i = 0; i < packet.getRespSize(); i++) {
		Serial.print(packet.getResponse()[i], HEX);
		Serial.print(" ");
		Serial.println(packet.getRespSize());
	}
	client.write(packet.getResponse(), packet.getRespSize());
	client.write(FrameInfo::footer, 4);
	client.flush();
}

bool HClient::sendPacket(HPacket* packet) {
	if (!isConnected()) {delete packet; return false;}
	for (auto val : packets)
		if (val->getCMD() == packet->getCMD())
			return false;
	if (packet->isSegmented()) {
		for (auto segment: packet->getSegments()->segments) {
			client.write(segment.data, segment.size);
			client.flush();
		}
	}
	else {
		client.write(packet->getBytes(), packet->getSize());
		client.flush();
	}
	client.write(FrameInfo::footer, 4);
	if (packet->getCMD() == 0x02) delete packet;
	else packets.pushBack(packet);
	return true;
}
bool HClient::checkConnection() {
	if (!client.connected()) {
		if (!client.connect(serverAddress.c_str(), 4223)) {
			goutput.warning("!CLIENTDOWN");
			goutput.removeOutputable(this);
			if (!lostConnection) {
				lostConnection = true;
				connectionStateChanged.emit(false);
			}
			return false;
		}
		else if (lostConnection) {
			lostConnection = false;
			connectionStateChanged.emit(true);
		}
	}
	goutput.feedOutputable(this);
	return true;
}
void HClient::work()
{
	checkForTimeouts();
	receiveData();
}

void HClient::sendPing() {
	if (checkConnection()) sendPacket(PingPacket::create());
}

bool HClient::isConnected()
{
	return !lostConnection;
}

void HClient::print(const String& text)
{
	if (!isConnected()) return;
	sendPacket(TextPacket::create(text));
	Serial.println(text);
}

bool HClient::isAvailableForIMG()
{
	return availableForIMGSending;
}

void HClient::checkForTimeouts() {
	for (auto &packet: packets) {
		if (packet->getLastRespTime() + Global::packetTimeout < millis()) {
			goutput.warning("!TOUT: "+ packet->getName());
			if (packet->getCMD() == 0x01) availableForIMGSending = true;
			if(packets.remove(packet)) delete packet;
		}
	}
}

void HClient::receiveData() {
	if (client.available()) {
		List<uint8_t> list;
		while (client.available()) 
			list.pushBack(static_cast<uint8_t>(client.read()));
		

		if (list.count() > 0) {
			pingTimer.restart();
			auto cmd = list.getFront();
			list.popFront();
			if (cmd < 8) {
				for (auto& packet : packets) {
					if (packet->getCMD() == cmd) {
						packets.remove(packet);
						packet->responseReady(list);
						if (cmd == 0x01) availableForIMGSending = true;
					}
				}
			}
			else {
				if (cmd == 8) {
					char* dataToSend=nullptr;
					auto size = HSettings::get().toBytes(dataToSend);
					client.write(dataToSend, size);
					client.flush();
				}
				/*for (auto& packet : inputPackets) {
					if (packet.getCMD() == cmd) {
						packet.handle(list);
						sendResponse(packet);
					}
				}*/
			}
			
			for (auto item : list) {
				Serial.print(item, HEX);
				Serial.print(" ");
			}
			Serial.println("");
		}
	}
}