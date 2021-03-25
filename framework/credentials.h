#pragma once
class Credentials {
public:
	IPAddress staticIP;
	String ssid;
	String pass;
	IPAddress gateway;
	IPAddress subnet;
	IPAddress primaryDNS;
	IPAddress secondaryDNS;
	Credentials(const IPAddress& addr, const String& ssid, const String& pass) :
		gateway(192, 168, 0, 1),
		subnet(255, 255, 255, 0),
		primaryDNS(8, 8, 8, 8),
		secondaryDNS(8, 8, 4, 4),
		staticIP(addr), ssid(ssid), pass(pass) {}
};
