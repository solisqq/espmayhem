#include "out.h"

Out::Out()
{
	//Serial.begin(115200);
}

void Out::show(const Type& type, const String& toShow)
{
	String pre = typeToString(type);
	Serial.println(pre+toShow+")/");
	if (type == Type::Error) {
		delay(2000);
		ESP.restart();
	}
}

String Out::typeToString(const Type& type)
{
	switch(type){
	case Type::Info: return "?(";
	case Type::Debug: return "*(";
	case Type::Warning: return ".(";
	case Type::Error: return "!(";
	}
}
Out out;

