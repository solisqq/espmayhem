#include "maintain.h"


Maintain::Maintain()
{
	Serial.begin(115200);
	feedOutputable(&serialOutput);
}

void Maintain::feedOutputable(Outputable* out)
{
	if(!outputables.remove(out)) 
		outputables.pushBack(out);
}

void Maintain::removeOutputable(Outputable* out)
{
	outputables.removeAll(out);
}

void Maintain::error(const String& err) {
	for (auto out : outputables) out->print(err);
	ESP.restart();
}

void Maintain::warning(const String& warr) {
	for (auto out : outputables) out->print(warr);
}

void Maintain::info(const String& info) {
	for (auto out : outputables) out->print(info);
}

void Maintain::SerialAdapter::print(const String& text)
{
	Serial.println(text);
}

Maintain goutput;


