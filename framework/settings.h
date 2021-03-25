#pragma once
#include <Preferences.h>
#include "basicoutput.h"
#include "list.h"
#include "esp_camera.h"
#include "hutils.h"
#include "maintain.h"

namespace lockPref {
	Preferences preferences;
	template<typename T> uint8_t getVarType();
	template<> uint8_t getVarType<int>() { return 1; }
	template<> uint8_t getVarType<float>() { return 2; }
	template<> uint8_t getVarType<double>() { return 3; }
	template<> uint8_t getVarType<bool>() { return 4; }
	template<> uint8_t getVarType<String>() { return 5; }
	template<> uint8_t getVarType<char>() { return 6; }
	template<> uint8_t getVarType<uint8_t>() { return 7; }
};

class HSettings {

	class Storable {
		String name;
	protected:
		Storable(const String& name, List<Storable*>& storables): name(name) { storables.pushBack(this); }
	public:
		virtual void save() = 0;
		virtual void load()=0;
		String getName() { return name; };
		virtual bool setByBytes(const List<uint8_t>&)=0;
		virtual List<uint8_t> toBytes() = 0;
	};
	template <typename Type>
	class SVariable: public Storable {
		Type val;
		void save() override;
	public:
		SVariable<Type>(const String& name, Type defaultValue, List<Storable*>& storables): Storable(name, storables), val(defaultValue) {}
		void set(Type value) {
			val = value;
			save();
		}
		Type get() {return val;}
		void load() override;
		bool setByBytes(const List<uint8_t>&) override;
		List<uint8_t> toBytes() override;
	};
	List<Storable*> storables;
public:
	SVariable<uint8_t> imgQuality = SVariable<uint8_t>("imgQuality", 5, storables);
	SVariable<uint8_t> imgSize = SVariable<uint8_t>("imgSize", FRAMESIZE_SVGA, storables);

protected:
	HSettings(){lockPref::preferences.begin("my-app", false);}
	static HSettings* settings;
public:
	HSettings(HSettings& other) = delete;
	void operator=(const HSettings&) = delete;
	static HSettings& get() {
		if (settings == nullptr) settings = new HSettings();
		return *settings;
	}
	bool setValByID(uint8_t id, const List<uint8_t>& data) {
		if (id >= storables.count()) return false;
		if (!storables[id]->setByBytes(data)) return false;
		return true;
	}
	int toBytes(char* container) {
		List<uint8_t> dataToRet;
		dataToRet.pushBack(0x08);
		for (auto storable : storables) {
			List<uint8_t> data = storable->toBytes();
			dataToRet.pushBack(data.count());
			for (auto byte : data) dataToRet.pushBack(byte);
		}
		container = new char[dataToRet.count()];
		int i = 0;
		for (auto item : dataToRet) {
			container[i] = *(char*)&item;
			i++;
		}
		return dataToRet.count();
	}
	void overwriteWithDefault() {
		for (auto storable : storables) storable->save();
	}
	void loadDataFromMEM() {
		for (auto storable : storables) storable->load();
	}
	const List<Storable*>& getStorables() { return storables; }
};

HSettings* HSettings::settings = nullptr;

template<> bool HSettings::SVariable<int>::setByBytes(const List<uint8_t>& bytes) {
	if (bytes.count() != 4) return false;
	set(bytes_utils::FromBytesList<int>(bytes));
	return true;
}

template<> bool HSettings::SVariable<float>::setByBytes(const List<uint8_t>& bytes) {
	if (bytes.count() != 4) return false;
	set(bytes_utils::FromBytesList<float>(bytes));
	return true;
}

template<> bool HSettings::SVariable<double>::setByBytes(const List<uint8_t>& bytes) {
	if (bytes.count() != 8) return false;
	set(bytes_utils::FromBytesList<double>(bytes));
	return true;
}

template<> bool HSettings::SVariable<bool>::setByBytes(const List<uint8_t>& bytes) {
	if (bytes.count() != 1) return false;
	if (bytes.at(0) != 0) set(true);
	else set(false);
	return true;
}

template<> bool HSettings::SVariable<String>::setByBytes(const List<uint8_t>& bytes) {
	String valueToSave="";
	for (auto byte : bytes) 
		valueToSave += (char)byte;
	set(valueToSave);
	return true;
}

template<> bool HSettings::SVariable<char>::setByBytes(const List<uint8_t>& bytes) {
	if (bytes.count() != 1) return false;
	set((char)bytes.at(0));
	return true;
}

template<> bool HSettings::SVariable<uint8_t>::setByBytes(const List<uint8_t>& bytes) {
	if (bytes.count() != 1) return false;
	set(bytes.at(0));
	goutput.info(String(static_cast<int>(bytes.at(0))));
	return true;
}

template<> List<uint8_t> HSettings::SVariable<uint8_t>::toBytes() {
	List<uint8_t> bytes;
	bytes.pushBack(lockPref::getVarType<uint8_t>());
	bytes.pushBack(get());
	return bytes;
}

template<> void HSettings::SVariable<int>::save() { lockPref::preferences.putInt(getName().c_str(), val); }
template<> void HSettings::SVariable<float>::save() { lockPref::preferences.putFloat(getName().c_str(), val); }
template<> void HSettings::SVariable<double>::save() { lockPref::preferences.putDouble(getName().c_str(), val); }
template<> void HSettings::SVariable<bool>::save() { lockPref::preferences.putBool(getName().c_str(), val); }
template<> void HSettings::SVariable<String>::save() { lockPref::preferences.putString(getName().c_str(), val); }
template<> void HSettings::SVariable<char>::save() { lockPref::preferences.putChar(getName().c_str(), val); }
template<> void HSettings::SVariable<uint8_t>::save() { lockPref::preferences.putUInt(getName().c_str(), val); }

template<> void HSettings::SVariable<int>::load() { val = lockPref::preferences.getInt(getName().c_str(), 0); }
template<> void HSettings::SVariable<float>::load() { val = lockPref::preferences.getFloat(getName().c_str(), 0); }
template<> void HSettings::SVariable<double>::load() { val = lockPref::preferences.getDouble(getName().c_str(), 0); }
template<> void HSettings::SVariable<bool>::load() { val = lockPref::preferences.getBool(getName().c_str(), 0); }
template<> void HSettings::SVariable<String>::load() { val = lockPref::preferences.getString(getName().c_str(), ""); }
template<> void HSettings::SVariable<char>::load() { val = lockPref::preferences.getChar(getName().c_str(), 0); }
template<> void HSettings::SVariable<uint8_t>::load() { val = lockPref::preferences.getUInt(getName().c_str(), 0); }

namespace Global {
	int missedPingCounter = 0;
	int maxMissedPings = 3;
	int packetTimeout=500;

	namespace Netw {
		int reconnectAttempts = 9;
		int pingInterval = 1000;
	};

	namespace Pins {
		int OLED_SCL = 13;
		int OLED_SDA = 14;
		int LASER = 15;
		int BUZZER = 12;
		int TRIGGER = 16;
		int RESTART = 4;
	};
	namespace Peripherals {
		BasicOutput laser(Global::Pins::LASER, true, 500);
		BasicOutput buzzer(Global::Pins::BUZZER);
		BasicOutput hwrestart(Global::Pins::RESTART);
	};
};

#ifdef ARDUINO_IDE 
#include "settings.cpp" 
#endif
//#endif