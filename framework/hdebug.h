#pragma once
#include "Arduino.h"
#include "outputable.h"

class HDebug {
	Outputable* out{};
public:
	HDebug(Outputable* output) : out(output) {}
	void print(const String& text){if(out) out->print(text);}
};

extern HDebug hdebug;
