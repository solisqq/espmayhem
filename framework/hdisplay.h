#pragma once
#include "Arduino.h"
#include "hprocess.h"
#include <Wire.h>
#include "oled/SSD1306.h"
#include "outputable.h"
#include <Adafruit_SSD1306.h>
#include "maintain.h"

class HDisplay : public Process, public Outputable {
	Adafruit_SSD1306* display = nullptr;
	uint8_t sda;
	uint8_t scl;
public:
	HDisplay(uint8_t sda, uint8_t scl) :Process("display", GlobalWorkerHandler::Priority::Normal), sda(sda), scl(scl) {}
	uint8_t init() override {
		Wire.begin(sda, scl);
		display = new Adafruit_SSD1306(128, 32, &Wire, -1);
		if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) return 1;
		display->clearDisplay();
		goutput.feedOutputable(this);
		/*display->setTextSize(2); // Draw 2X-scale text
		display->setTextColor(SSD1306_WHITE);
		display->setCursor(10, 0);
		display->println("scroll");
		display->display();*/
		return 0;
	}
	void print(const String& text) override {
		display->clearDisplay();
		display->setTextSize(1); // Draw 2X-scale text
		display->setTextColor(SSD1306_WHITE);
		display->setCursor(4, 24);
		display->println(text);
		display->display();
	}
	void work() {}
};

/*
class HDisplay: public Process {
	SSD1306  display;
public:
	HDisplay(uint8_t sda, uint8_t scl):Process("display", GlobalWorkerHandler::Priority::Normal), display(0x3c, sda, scl, GEOMETRY_128_32){}
	uint8_t init() override {
		if (!display.init()) return 1;
		display.flipScreenVertically();
		display.setTextAlignment(TEXT_ALIGN_LEFT);
		display.setFont(ArialMT_Plain_10);
		display.drawString(0, 0, "Hello world");
		return 0;
	}
	void work() override {}
};
*/

#ifdef ARDUINO_IDE 
#include "hdisplay.cpp" 
#endif
