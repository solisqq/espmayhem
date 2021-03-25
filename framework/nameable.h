#pragma once
#include <WString.h>

class Nameable
{
	String name="";
protected:
	Nameable() {};
	Nameable(const String& name):name(name){}
	void setName(const String& _name) { name = _name; }
	String getName() { return name; }
};

