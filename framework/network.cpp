#include "network.h"

uint8_t Network::init()
{
	WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS);
	int failCounter = Global::Netw::reconnectAttempts;
	WiFi.begin(ssid.c_str(), pass.c_str());
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		failCounter--;
		if (failCounter <= 0) {
			return 1;
		}
	}
	goutput.info(WiFi.localIP().toString());
	return 0;
}

Network::Network(const IPAddress& addr, const String& ssid, const String& pass) :
	Process("network", GlobalWorkerHandler::Priority::Normal),
	Credentials(addr,ssid,pass){}

void Network::work()
{
	if (WiFi.status() == WL_CONNECTED) return;
	WiFi.disconnect();
	WiFi.begin(ssid.c_str(), pass.c_str());
}
