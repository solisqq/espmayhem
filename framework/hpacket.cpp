#include "hpacket.h"
#include "settings.h"

ImagePacket* ImagePacket::create(camera_fb_t* img)
{
	return new ImagePacket(img);
}

PingPacket* PingPacket::create()
{
	return new PingPacket();
}

TextPacket* TextPacket::create(const String& text)
{
	return new TextPacket(text);
}
