#include "hutils.h"
#include "list.h"
#define _BAD_VAL -144

List<char> bytes_utils::toBytesList(char* chars, int size) {
	List<char> toRet;
	for (int i = 0; i < size; i++)
		toRet.pushBack(chars[i]);
	return toRet;
}

int bytes_utils::ListToBytesArray(const List<char>& chars, char* arr) {
	arr = new char[chars.count()];
	int i = 0;
	for (auto item : chars) {
		arr[i] = item;
		i++;
	}
}

char* bytes_utils::concatBytesArray(const char* first, int first_size, const char* second, int second_size) {
	char* toRet = new char[first_size + second_size];
	memcpy(toRet, first, first_size);
	//memcpy(toRet+first_size, )
	return toRet;
}
void bytes_utils::pushBytes(char* to, const char* from, int howMuch, int& counter) {
	memcpy(to + counter, from, howMuch);
	counter += howMuch;
}
void bytes_utils::toByteArray(int toConvert, char* destination) {
	destination[0] = toConvert & 0x000000ff;
	destination[1] = (toConvert & 0x0000ff00) >> 8;
	destination[2] = (toConvert & 0x00ff0000) >> 16;
	destination[3] = (toConvert & 0xff000000) >> 24;
}

