#pragma once
#include "hprocess.h"
#include "WiFi.h"
#include "WString.h"
#include "hpacket.h"
#include "esp_wifi.h"
#include "settings.h"
#include "outputable.h"
#include "maintain.h"
#include "timer.h"

class HClient : public Process, public Outputable {
	List<HPacket*> packets;
	List<HInputPacket&> inputPackets;

	WiFiClient client;
	String serverAddress;
	bool lostConnection = true;
	Timer pingTimer;
	void sendPing();
	uint8_t init() override;
	void receiveData();
	void checkForTimeouts();
	bool checkConnection();
	bool availableForIMGSending = true;

	RequestSettingsPacket settingsPacket;
	RequestSetupPacket setupPacket;
	void sendResponse(HInputPacket& packet);
public:
	Signal<void, bool> connectionStateChanged;
	HClient(const String& ip);
	void sendIMG(camera_fb_t* img);
	bool sendPacket(HPacket* packet);
	void work() override;
	bool isConnected();
	void print(const String& text) override;
	bool isAvailableForIMG();
};
#ifdef ARDUINO_IDE 
#include "hclient.cpp" 
#endif
