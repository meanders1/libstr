#include "libstr.hpp"

int testStr() {
	constexpr int antTester = 10;
	float aX = 0.23;
	float aY = -1.12;
	float aZ = 0.03;
	float pressure = 98.34;
	float gX = 0.1;
	float gY = 0.11;
	float gZ = 2.3;
	long total = 0;

    Str<6+1 + 8+1+8+1+8+1 + 8+1 + 8+1+8+1+8> string(',');
	for (int i = 0; i < antTester; i++) {
		unsigned long start = millis();
		for (int j = 0; j < 500; j++) {

			string.padSet(0, 6, start);
			string.set(6, ';');

			string.padSetLF(7, 8, 2, aX);
			string.set(15, ';');
			string.padSetLF(16, 8, 2, aY);
			string.set(24, ';');
			string.padSetLF(25, 8, 2, aZ);
			string.set(33, ';');

			string.padSetLF(34, 8, 2, pressure);
			string.set(42, ';');

			string.padSetLF(43, 8, 2, gX);
			string.set(51, ';');
			string.padSetLF(52, 8, 2, gY);
			string.set(60, ';');
			string.padSetLF(61, 8, 2, gZ);
		}

		unsigned long end = millis();
		total += end - start;
	}
	Serial.println(string.buffer);
	total /= antTester;
	return (int) total;
}

int testArduinoString() {
	// String string;
	// string.reserve(43); // Does not help much, as the string is not concatenated to, only assigned to.
	constexpr int antTester = 10;
	float aX = 0.23;
	float aY = -1.12;
	float aZ = 0.03;
	float pressure = 98.34;
	float gX = 0.1;
	float gY = 0.11;
	float gZ = 2.3;
	long total = 0;
    String string;
	for (int i = 0; i < antTester; i++) {
		unsigned long start = millis();
		for (int j = 0; j < 500; j++) {
			string = String(start) + ";" + String(aX) + ";" + String(aY) + ";" + String(aZ) + ";" + String(pressure) + ";" + String(gX) + ";" + String(gY) + ";" + String(gZ);
		}
		unsigned long end = millis();
		total += end - start;
	}
	Serial.println(string);
	total /= antTester;
	return (int) total;
}

int testUL() {
	Str<20> string('-');
	unsigned long val = 1020304;
	int total = 0;
	for (int i = 0; i < 100; i++) {
		unsigned long start = millis();
		for (int i = 0; i < 1000; i++) {
			string.set(2, val);
		}
		unsigned long end = millis();
		total += end - start;
	}
	Serial.println(string.buffer);
	return total / 100;
}

int testPadUL() {
	Str<20> string('-');
	unsigned long val = 1020304;
	int total = 0;
	for (int i = 0; i < 10; i++) {
		unsigned long start = millis();
		for (int i = 0; i < 1000; i++) {
			string.padSet(2, 9, val);
		}
		unsigned long end = millis();
		total += end - start;
	}
	Serial.println(string.buffer);
	return total / 100;
}

float testF() {
	Str<14> string('-');
	constexpr int numTests = 1;
	float val = 10.34519;
	int total = 0;
    int returnCode = 0;
	for (int i = 0; i < numTests; i++) {
		unsigned long start = millis();
		for (int i = 0; i < 1; i++) {
			returnCode = string.padSetLF(2, 12, 5, val);
		}
		unsigned long end = millis();
		total += end - start;
	}
    Serial.print("Return code: ");
    Serial.println(returnCode);
	Serial.println(string.buffer);
	return (float)total / numTests;
}

float testPadF() {
	Str<14> string('-');
	constexpr int numTests = 1;
	float val = 12.34519;
	int total = 0;
    int returnCode = 0;
	for (int i = 0; i < numTests; i++) {
		unsigned long start = millis();
		for (int i = 0; i < 1; i++) {
			returnCode = string.padSet(2, 8, val);
		}
		unsigned long end = millis();
		total += end - start;
	}
    Serial.print("Return code: ");
    Serial.println(returnCode);
	Serial.println(string.buffer);
	return (float)total / numTests;
}

void setup() {
	Serial.begin(9600);
}

void loop() {
	int strPerf = testStr();
	int aPerf = testArduinoString();
	Serial.print("Str average time: ");
	Serial.print(strPerf);
	Serial.println("ms");

	Serial.print("Arduino string average time: ");
	Serial.print(aPerf);
	Serial.println("ms");
    Serial.println();

	// Serial.println("Hello");

	// Str<100> str(',');

	// char theStr[] = "0ge";
	// int theInt = 123;
	// float theFloat = 3522.12120;
	// unsigned long theULong = 65;
	// float secFloat = -2.354;

	// int failure1 = 0; //str.set(1, 3, theStr);
	// int failure2 = 0; //str.set<3>(5, theInt);
	// int failure3 = str.set<5>(10, theFloat);
	// int failure4 = 0; //str.padSet<5>(18, theULong);
	// int failure5 = 0; //str.padSet<3>(24, secFloat);
	// Serial.print("Str:(");
	// Serial.print(str.buffer);
	// Serial.println(")");

	// if (failure1 || failure2 || failure3 || failure4 || failure5) {
	// 	Serial.print("Error code 1: ");
	// 	Serial.println(failure1);
	// 	Serial.print("Error code 2: ");
	// 	Serial.println(failure2);
	// 	Serial.print("Error code 3: ");
	// 	Serial.println(failure3);
	// 	Serial.print("Error code 4: ");
	// 	Serial.println(failure4);
	//     Serial.print("Error code 5: ");
	//     Serial.println(failure5);
	// }
}
