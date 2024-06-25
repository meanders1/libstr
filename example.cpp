#include <iostream>

#include "libstr.hpp"

template<typename T> 
void println(T val) {
	std::cout << val << std::endl;
}

int main() {
	Str<16> str('-');
	unsigned long thelong = 12345678;
	str.padSet(2, 8, thelong);

	println(str.buffer);
	println(str.length());
	return 0;
}
