/*
For unsafe mode (strongly discouraged), define LIBSTR_UNSAFE before including this file.
Using unsafe mode is only marginally faster, and disables checks that can stop the program from crashing.

Example: To define unsafe mode write (at the top of a file):
... other includes ...
#define LIBSTR_UNSAFE
#include "libstr.hpp"
... rest of the code ...

*/

#include <cmath>

static inline float absf(float f) {
	/* optimizer will optimize away the `if` statement and the library call */
	if (sizeof(float) == sizeof(uint32_t)) {
		union {
			float f;
			uint32_t i;
		} u;
		u.f = f;
		u.i &= 0x7fffffff;
		return u.f;
	}
	return abs(f);
}

inline int powl(int base, unsigned int exponent) {
	int total = 1;
	for (unsigned int i = 0; i < exponent; i++) {
		total *= base;
	}
	return total;
}

template<uint_fast16_t buffSize>
class Str {
  public:
	char buffer[buffSize + 1];
  public:
	// Construct string. The characters in the string can be anything.
	Str() {
		buffer[buffSize] = '\0';
	}

	// Copy contructor
	Str(const Str<buffSize>& other) {
		memcpy(buffer, other.buffer, buffSize + 1);
	}

	// Creates a string filled with fillCharacter
	Str(char fillCharacter) {
		fill(fillCharacter);
	}

	// Repeats the character in the span [start, end>
	// returns:
	//   0 if success
	//  -1 if end > the size of this Str
	int set(uint_fast16_t start, uint_fast16_t end, char character) {
#ifndef LIBSTR_UNSAFE
		if (end > buffSize) {
			return -1;
		}
#endif
		for (uint_fast16_t i = start; i < end; i++) {
			buffer[i] = character;
		}
		return 0;
	}

	// Fills the Str with the spesified character, and appends a null terminator
	void fill(char character) {
		for (uint_fast16_t i = 0; i < buffSize; i++) {
			buffer[i] = character;
		}
		buffer[buffSize] = '\0';
	}

	// Copies characters from the data string to this Str.
	// Returns:
	//   0 if successful
	//  -1 if out of bounds
	// startIndex: the index of this Str to start writing.
	// numChars: the amount of characters to copy from data
	// data: the string to copy from
	int set(uint_fast16_t startIndex, uint_fast16_t numChars, const char* data) {
#ifndef LIBSTR_UNSAFE
		if (startIndex + numChars > buffSize) {
			return -1;	// String out of bounds
		}
#endif
		memcpy(buffer + startIndex, data, numChars);
		return 0;
	}

	// Sets one character at the specified index
	// Returns:
	//   0 if successful
	//  -1 if out of bounds
	int set(uint_fast16_t index, char data) {
#ifndef LIBSTR_UNSAFE
		if (index > buffSize) {
			return -1;	// Index out of bounds
		}
#endif
		buffer[index] = data;
		return 0;
	}

	// Writes a unsigned long into this str, and pads the left side with 0s
	// Returns:
	//  0 if success
	// -1 if start + numChars is out of bounds
	// -2 data has more digits than numChars
	// Params:
	// numChars:
	//   the amount of characters used.
	// start:
	//   the index in this Str to start writing the unsigned long
	// data:
	//   the unsigned long to be written
	int padSet(uint_fast16_t start, uint_fast16_t numChars, unsigned long data) {
		// Calculate number of digits
		uint_fast8_t numDigits = 0;
		unsigned long num = data;
		do {
			num = num / 10;
			numDigits++;
		} while (num != 0);

#ifndef LIBSTR_UNSAFE
		if (numDigits > numChars) {
			return -2;	// Number of digits is too large
		}
		if (start + numChars >= buffSize) {
			return -1;	// Out of bounds
		}
#endif
		// Write the long from right to left
		for (uint_fast8_t i = 0; i < numDigits; i++) {
			// Finding the last character by adding the last digit to the ASCII code of '0'(=48)
			char c = '0' + (data % 10);
			buffer[start + (numChars - 1 - i)] = c;
			// Shift the long to the right, so the second last digit is the last
			data /= 10;
		}
		// Write the padding
		for (uint_fast8_t i = 0; i < numChars - numDigits; i++) {
			buffer[start + i] = '0';
		}
		return 0;
	}

	// Writes a longfloat with padding. A longfloat is a format for storing floats which is faster to write than a regular float
	// For example: 12.3 -> +1231 the 1 one at the end (called the decimal character) indicates how many digits are left of the decimal point
    // Another example: -12.3 -> -1231 
	// The padding is a series of zeros on the left of the float, whose purpose is to fill the amount of characters indicated by numChars
	// Example:
	// padSetLF(0, 8, 2, 123.456) -> 0+123452 (the 2 at the end is the decimals character)
	// params:
	//  - start, where to start writing the longfloat
	//  - numChars, the total number of characters to write
	//  - numDecimals, the amount of decimals to include in the longfloat
	//  - data, the float to write as a longfloat
	// returns: the success code
	//   0, if success
	//  -1, if start + numChars > the size of the Str
	//  -2, if the amount of digits in the longfloat + 2 (adding 1 for decimal character and 1 for prefix(+/-)) > numChars
	//  -3, if numDecimals + 2 (adding 1 for decimal character and 1 for prefix(+/-)) > numChars
	int padSetLF(uint_fast16_t start, uint_fast8_t numChars, uint_fast8_t numDecimals, float data) {
#ifndef LIBSTR_UNSAFE
		if (numDecimals + 2 > numChars) { // adding 1 for decimal character and 1 for prefix(+/-)
			return -3;	// numDecimals is too large
		}
#endif
		unsigned long num = (long)(absf(data) * powl(10, numDecimals));

		uint_fast8_t numDigits = 0;	 // Total number of digits in num
		unsigned long tempNum = num;
		do {
			tempNum = tempNum / 10;
			numDigits++;
		} while (tempNum != 0);

#ifndef LIBSTR_UNSAFE
		if (numDigits + 2 > numChars) { // adding 1 for decimal character and 1 for prefix(+/-)
			return -2;
		}
		if (start + numChars > buffSize) {
			return -1;
		}
#endif
		// Write the padding
		for (uint_fast8_t i = 0; i < numChars - numDigits - 2; i++) { // subtracting 1 for decimal character and 1 for prefix(+/-)
			buffer[start + i] = '0';
		}

		// Write the longfloat
        uint_fast16_t index = start + numChars - numDigits - 2;
        if (data < 0) {
            buffer[index] = '-';
        } else {
            buffer[index] = '+';
        }

		for (int_fast16_t i = 0; i < numDigits; i++) {
			// Finding the last character by adding the last digit to the ASCII code of '0'(=48)
			char c = '0' + (num % 10);
			buffer[start + (numChars-i-2)] = c;
			// Shift the long to the right, so the second last digit is the last
			num /= 10;
		}

		// Write the amount of decimals
		buffer[start + numChars - 1] = '0' + numDecimals;
		return 0;
	}
};