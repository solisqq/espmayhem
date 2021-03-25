#pragma once
#include "Arduino.h"
#include "list.h"
#include "outputable.h"

class Maintain {
	List<Outputable*> outputables;
	class SerialAdapter : public Outputable {
	public:
		void print(const String& text) override;
	};
	SerialAdapter serialOutput;
public:
	Maintain();
	void feedOutputable(Outputable* out);
	void removeOutputable(Outputable* out);
	void error(const String& err);
	void warning(const String& warr);
	void info(const String& info);
};

extern Maintain goutput;

#ifdef ARDUINO_IDE 
#include "maintain.cpp" 
#endif
