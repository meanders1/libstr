﻿#include <iostream>

#include "libstr.hpp"
using namespace libstr;

template<typename T>
void println(T val);
template<typename T>
void print(T val);

template<fu16 theSize>
std::ostream& operator<<(std::ostream& os, const Str<theSize>& str) {
	for (fu16 i = 0; i < theSize; i++) {
		char c = str[i];
		if (c == '\0') {
			os << "\\0";
		}
		else if (c == '\n') {
			os << "\\n";
		}
		else {
			os << c;

		}
	}
	return os;
}

static void demonstrate() {
	println("Format: \nresult | success code");

	println("\n ---- Init no arguments ---- ");
	{
		Str<0> str;
		print(str);
		println(" | [none]");
	}
	{
		Str<4> str;
		print(str);
		println(" | [none]");
	}

	println("\n ---- Init copy ---- ");
	{
		Str<8> other('!');
		Str<8> str(other);
		print(str);
		println(" | [none]");
	}
	{
		Str<0> other('!');
		Str<0> str(other);
		print(str);
		println(" | [none]");
	}

	println("\n ---- Init character ---- ");
	{
		Str<4> str('!');
		print(str);
		println(" | [none]");
	}
	{
		Str<0> str('!');
		print(str);
		println(" | [none]");
	}
	{
		Str<4> str('\0');
		print(str);
		println(" | [none]");
	}

	println("\n ---- Set char ---- ");
	{
		char c = 'o';
		Str<4> str('!');
		int success = str.set(0, c);
		print(str);
		print(" | ");
		println(success);
	}
	{
		char c = 'o';
		Str<4> str('!');
		int success = str.set(4, c);
		print(str);
		print(" | ");
		println(success);
	}
	{
		char c = 'o';
		Str<0> str('!');
		int success = str.set(0, c);
		print(str);
		print(" | ");
		println(success);
	}

	println("\n ---- Set char span ---- ");
	{
		Str<8> str('!');
		int success = str.set(1, 6, 'o');
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<2> str('!');
		int success = str.set(3, 6, 'o');
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		int success = str.set(0, 9, 'o');
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		int success = str.set(6, 1, 'o');
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		int success = str.set(1, 1, 'o');
		print(str);
		print(" | ");
		println(success);
	}

	println("\n ---- Fill ----");
	{
		Str<8> str('!');
		str.fill('o');
		print(str);
		println(" | [none]");
	}
	{
		Str<0> str('!');
		str.fill('o');
		print(str);
		println(" | [none]");
	}

	println("\n ---- Set const char* ----");
	{
		Str<8> str('!');
		const char* buff = "text";
		int success = str.set(1, 4, buff);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		const char* buff = "text_text";
		int success = str.set(1, 7, buff);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		const char* buff = "text_text";
		int success = str.set(0, 9, buff);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		const char* buff = "text";
		int success = str.set(1, 6, buff);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<0> str('!');
		const char* buff = "";
		int success = str.set(0, 0, buff);
		print(str);
		print(" | ");
		println(success);
	}

	println("\n ---- Padset unsigned long ---- ");
	{
		Str<8> str('!');
		unsigned long thelong = 12345678;
		int success = str.padSet(0, 8, thelong);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		unsigned long thelong = 12345678;
		int success = str.padSet(0, 7, thelong);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		unsigned long thelong = 123;
		int success = str.padSet(1, 6, thelong);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<0> str('!');
		unsigned long thelong = 1234;
		int success = str.padSet(0, 8, thelong);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<4> str('!');
		unsigned long thelong = 12345678;
		int success = str.padSet(0, 8, thelong);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		unsigned long thelong = 9;
		int success = str.padSet(8, 1, thelong);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		unsigned long thelong = 12345678;
		int success = str.padSet(0, 8, thelong);
		print(str);
		print(" | ");
		println(success);
	}

	println("\n ---- Padset long ---- ");
	{
		Str<8> str('!');
		long thelong = -1234567;
		int success = str.padSet(0, 8, thelong);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		long thelong = -1234567;
		int success = str.padSet(0, 7, thelong);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		long thelong = -123456;
		int success = str.padSet(3, 7, thelong);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<8> str('!');
		long thelong = 1234;
		int success = str.padSet(2, 6, thelong);
		print(str); 
		print(" | ");
		println(success);
	}

	println("\n ---- Padset float ---- ");
	{
		Str<6> str('!');
		float thefloat = +12.34;
		int success = str.padSet(0, 6, 2, thefloat);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<5> str('!');
		float thefloat = +12.34;
		int success = str.padSet(0, 6, 2, thefloat);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<6> str('!');
		float thefloat = +12.34;
		int success = str.padSet(1, 6, 2, thefloat);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<6> str('!');
		float thefloat = +12.34;
		int success = str.padSet(0, 6, 3, thefloat);
		print(str);
		print(" | ");
		println(success);
	}
	{
		Str<16> str('!');
		float thefloat = +12.34;
		int success = str.padSet(0, 16, 6, thefloat);
		print(str);
		print(" | ");
		println(success);
	}
}

int main() {
	demonstrate();
	return 0;
}

template<typename T>
void println(T val) {
	std::cout << val << std::endl;
}

template<typename T>
void print(T val) {
	std::cout << val;
}
