#pragma once
#include "slotable.h"
#include "WString.h"
#include "hutils.h"
#include "settings.h"
#include "timer.h"
#include "hdebug.h"

namespace FrameInfo {
	const char header[4] = { 0xFF,0xFA,0x0A,0x01 };
	const char footer[4] = { 0xFF,0xFA,0x0A,0x01 };
	const char splitter[4] = { 0xFF,0x0B,0xFB,0x0A };
	
	constexpr unsigned int countHeader = 4;
	constexpr unsigned int countHash = 4;
	//constexpr unsigned int countData = 4096;
	constexpr unsigned int countData = 8192;
	constexpr unsigned int countFooter = 4;

	constexpr unsigned int posHeader = 0;
	constexpr unsigned int posCMD = 4;
	constexpr unsigned int posHash = 5;
	constexpr unsigned int posSegmentID = 9;
	constexpr unsigned int posSegmentALL = 10;
	constexpr unsigned int posData = 11;
	constexpr unsigned int posFooter = posData + countData;
	constexpr unsigned int posEnd = posFooter + 4;

	void pushHeader(char* tab, int cmd) {
		for (int i = 0; i < countHeader; i++) tab[i] = header[i];
		tab[posCMD] = cmd;
	}

	void pushDataAsSegment(char* destination, char* data, int dataSize, char segmentID, char cmdID, char allSegmentsCount) {
		for (int i = 0; i < 4; i++) destination[i] = header[i];//4
		destination[4] = cmdID;
		destination[9] = segmentID;
		destination[10] = allSegmentsCount;
		int hash=0;
		for (int i = 0; i < dataSize; i++) {
			destination[i + 11] = data[i];
			if (hash > 0) hash -= static_cast<int>(data[i]);
			else hash += static_cast<int>(data[i]);
		}
		/*char hashBytes[4];
		bytes_utils::toByteArray(abs(hash), hashBytes);
		for (int i = 0; i < 4; i++) {
			Serial.print(static_cast<uint8_t>(hashBytes[i]), HEX);
			Serial.print(" ");
		}
		Serial.println(hash);
		for (int i = 0; i < 4; i++) destination[5+i] = hashBytes[i];*/
		destination[5] = static_cast<char>(abs(hash));
		if(allSegmentsCount==segmentID) 
			for (int i = 0; i < 4; i++) destination[dataSize + i + 11] = footer[i];
		else
			for (int i = 0; i < 4; i++) destination[dataSize + i + 11] = splitter[i];
	}

	//segmented:
	//0xFF, 0xFA, 0x0A, 0x01,	0x01,			0x00, 0x00, 0x00, 0x00,				0x01,				0x05,				0xFF, 0xFA, 0x0A, 0x01
	//	header					cmd					4byte hash					segment id      of totall segments                 footer
	
	
	//non-segmented:
	//0xFF, 0xFA, 0x0A, 0x01,		0x01,		0x00, 0x00, 0x00, 0x00,			0x00,			0xFF, 0xFA, 0x0A, 0x01
	//	      header				 cmd			 4byte hash				data(optional)              footer
};

class Segments {
	class Segment {
	public:
		int size = 0;
		char* data=nullptr;
		Segment(char* dataptr, int count): data(dataptr), size(count){}
	};
public:
	List<Segment> segments;
	Segments(char* data, int size) {
		int count = size / FrameInfo::countData;
		int rest = size % FrameInfo::countData;
		for (int i = 0; i < count; i++) segments.pushBack(Segment(data + (i * FrameInfo::countData), FrameInfo::countData));
		if (rest > 0) segments.pushBack(Segment(data + (count * FrameInfo::countData), rest));
	}
};
class HPacket : public Slotable {
protected:
	uint8_t cmd;
	String cmdName = "";
	int size=0;
	Segments* segments=nullptr;
	char* toSend=nullptr;
	unsigned long respTime = 0;
	bool deleteBufferAfter = true;
public:
	~HPacket() { if (deleteBufferAfter) delete[] toSend; if (isSegmented()) delete segments; };
	uint8_t getCMD() const {return cmd;}
	virtual char* getBytes() {
		return toSend;
	}
	int getSize() {return size;}
	String getName() { return cmdName; }
	void responseReady(const List<uint8_t>& response) { handleResponse(response); delete this; }
	virtual void handleResponse(const List<uint8_t>& response) {}
	unsigned long getLastRespTime() { return respTime; }
	bool isSegmented() { return (segments != nullptr); }
	Segments* getSegments() { return segments; }
protected:
	HPacket(uint8_t cmd, const String& name):
		cmdName(name), cmd(cmd){
		respTime = millis();
	}
	
};

class PingPacket : public HPacket {
public:
	static PingPacket* create();
private:
	PingPacket(): HPacket(static_cast<uint8_t>(0x00), "ping") {
		size = 1;
		toSend = new char[size];
		toSend[0] = *(char*)&cmd;
	}
};

class ImagePacket : public HPacket {
public:
	static ImagePacket* create(camera_fb_t* img);
private:
	ImagePacket(camera_fb_t* img) : HPacket(static_cast<uint8_t>(0x01), "img") {
		size = img->len;
		deleteBufferAfter = false;
		segments = new Segments((char*)img->buf, img->len);
	}
};

class TextPacket : public HPacket {
public:
	static TextPacket* create(const String& text);
private:
	TextPacket(const String& text) : HPacket(static_cast<uint8_t>(0x02), "text") {
		size = text.length() + 1;
		toSend = new char[size];
		toSend[0] = cmd;
		text.toCharArray(toSend+1, text.length());
	}
};

class HInputPacket : public Slotable {
protected:
	uint8_t cmd;
	String cmdName = "";
	char* response=nullptr;
	int size=0;
public:
	~HInputPacket() { if (response) delete []response; };
	uint8_t getCMD() const { return cmd; }
	int getRespSize() { return size; }
	char* getResponse() { return response; }
	String getName() { return cmdName; }
	void handle(const List<uint8_t>& resp) {
		if (response) delete response;
		size = 0;
		handleData(resp);
	}
	
protected:
	virtual void handleData(const List<uint8_t>& response) = 0;
	HInputPacket(uint8_t cmd, const String& name) :cmdName(name), cmd(cmd) {}
};

class RequestSetupPacket : public HInputPacket {
public:
	RequestSetupPacket(): HInputPacket(0x09,"set"){}
private:
	void handleData(const List<uint8_t>& resp) override {
		auto id = resp.at(0);
		int byteCount = resp.at(1);
		if (resp.count() - 2 < byteCount) return;
		List<uint8_t> bytes;
		for (int i = 0; i < byteCount; i++) bytes.pushBack(resp.at(i+2));
		size = 2;
		response = new char[size];
		response[0] = 0x09;
		if (HSettings::get().setValByID(id, bytes)) response[1] = 0x01;
		else response[1] = 0x00;
	}
};

class RequestSettingsPacket : public HInputPacket {
public:
	~RequestSettingsPacket() {}
	RequestSettingsPacket() : HInputPacket(static_cast<uint8_t>(0x08), "reqset") {}
private:
	void handleData(const List<uint8_t>& resp) override {
		size = HSettings::get().toBytes(response);
	}
};


#ifdef ARDUINO_IDE 
#include "hpacket.cpp" 
#endif

/*class Segments {
public:
	bool shouldBeResended = false;
	class Segment {
		char* pointerToData;
		int size;
		char cmd;
		int id;
		int allCount;
	public:
		bool toSend = true;
		char data[FrameInfo::countData + 15];
		int getID() { return id; }
		int getSize() const { return size; }
		Segment(char* pointer, int size, char cmd, int id, int allCount) :
			pointerToData(pointer), size(size + 15), cmd(cmd), id(id + 1), allCount(allCount) {
			FrameInfo::pushDataAsSegment(data, pointer, size, static_cast<char>(getID()), static_cast<char>(cmd), allCount);
		}
	};
	List<Segment> segments;
	Segments(char* pointerToData, int size, char cmd) {
		int fullSegments = size / FrameInfo::countData;
		int mod = size % FrameInfo::countData;
		for (int i = 0; i < fullSegments; i++) {
			segments.pushBack(
				Segment(pointerToData + (i * FrameInfo::countData), FrameInfo::countData, cmd, i, fullSegments + (mod == 0 ? 0 : 1))
			);
		}
		if (mod != 0)
			segments.pushBack(Segment(pointerToData + (fullSegments * FrameInfo::countData), mod, cmd, fullSegments, fullSegments + 1));
	}
};*/

/*size = img->len+15;
		char* data = (char*)img->buf;
		toSend = new char[size+15];
		for (int i = 0; i < 4; i++) toSend[i] = FrameInfo::header[i];//4
		toSend[4] = 0x01;
		toSend[9] = 0x01;
		toSend[10] = 0x01;
		int hash = 0;
		for (int i = 0; i < img->len; i++) {
			toSend[i + 11] = data[i];
			if (hash > 0) hash -= static_cast<int>(data[i]);
			else hash += static_cast<int>(data[i]);
		}
		toSend[5] = static_cast<char>(abs(hash));
		for (int i = 0; i < 4; i++) toSend[img->len + i + 11] = FrameInfo::footer[i];*/

/*void responseReady(const List<char>& response) override {
		segments->shouldBeResended = false;
		for (auto sign : response) {
			if (sign != 0xFF) {
				segments->segments[static_cast<int>(sign)].toSend = true;
				segments->shouldBeResended = true;
			}
		}
		return;
		if(!segments->shouldBeResended) delete this;
	}*/