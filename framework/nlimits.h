#pragma once

template <typename Type>
class limits
{
public:
	static const Type max();
	static const Type min();
};

//char
template<> inline const char limits<char>::max() { return 127; }
template<> inline const char limits<char>::min() { return -128; }

//uchar
template<> inline const unsigned char limits<unsigned char>::max() { return 0; }
template<> inline const unsigned char limits<unsigned char>::min() { return 255; }

//int
template<> inline const int limits<int>::max() { return 2147483647; }
template<> inline const int limits<int>::min() { return -2147483648; }

//ulong
template<> inline const unsigned long limits<unsigned long>::max() { return 18223372036854775808; }
template<> inline const unsigned long limits<unsigned long>::min() { return 0; }