#pragma once
#ifndef HUTILS_H
#define HUTILS_H
#include "list.h"
namespace bytes_utils {
	List<char> toBytesList(char* chars, int size);
	int ListToBytesArray(const List<char>& chars, char* arr);
	template<typename Type>
	Type FromBytesList(const List<uint8_t>& chars, bool bigEndian=true);
	char* concatBytesArray(const char* first, int first_size, const char* second, int second_size);
	void pushBytes(char* to, const char* from, int howMuch, int& counter);
	void toByteArray(int toConvert, char* destination);

	template<>
	int FromBytesList<int>(const List<uint8_t>& chars, bool bigEndian)
	{
		if (chars.count() != 4) return 0;
		int toRet;
		if (bigEndian) toRet = (chars.at(0) << 24) | (chars.at(1) << 16) | (chars.at(2)) << 8 | (chars.at(3));
		else toRet = (chars.at(3) << 24) | (chars.at(2) << 16) | (chars.at(1)) << 8 | (chars.at(0));
		return toRet;
	}

	template<>
	float FromBytesList<float>(const List<uint8_t>& chars, bool bigEndian)
	{
		if (chars.count() != 4) return 0;
		float toRet;
		if (bigEndian) toRet = (chars.at(0) << 24) | (chars.at(1) << 16) | (chars.at(2)) << 8 | (chars.at(3));
		else toRet = (chars.at(3) << 24) | (chars.at(2) << 16) | (chars.at(1)) << 8 | (chars.at(0));
		return toRet;
	}

	template<>
	double FromBytesList<double>(const List<uint8_t>& chars, bool bigEndian)
	{
		if (chars.count() != 8) return 0;
		float toRet;
		if (bigEndian) toRet = (chars.at(0) << 24) | (chars.at(1) << 16) | (chars.at(2)) << 8 | (chars.at(3));
		else toRet = (chars.at(3) << 24) | (chars.at(2) << 16) | (chars.at(1)) << 8 | (chars.at(0));
		return toRet;
	}
}

#ifdef ARDUINO_IDE 
#include "hutils.cpp" 
#endif
#endif
