/*
For unsafe mode (strongly discouraged), define LIBSTR_UNSAFE before including this file.
Using unsafe mode is only occasionally faster, and disables checks that can stop the program from crashing.

Example: To use unsafe mode write (at the top of a file):
... other includes ...
#define LIBSTR_UNSAFE
#include "libstr.hpp"
... rest of the code ...

*/

#include <stdint.h>
#include <cmath>

namespace libstr {

	typedef uint_fast16_t fu16;
	typedef uint_fast8_t fu8;
	typedef uint32_t u32;

	template<typename T>
	static inline T abs(T v) {
		return (v >= 0) ? v : -v;
	}

	template<fu16 bufferSize>
	class Str {
	public:
		char buffer[bufferSize];

		inline fu16 length() {
			return bufferSize;
		}

		Str<bufferSize + 1> nullTerminated() {
			Str<bufferSize + 1> other;
			other.set(0, *this);
			other.set(other.length() - 1, '\0');
			return other;
		}
	public:
		// Construct empty string. The string's buffer is uninitialized, use another constructur to initialize the Str's buffer.
		Str() {}

		// Copy contructor
		Str(const Str<bufferSize>& other) {
			memcpy(buffer, other.buffer, static_cast<size_t>(bufferSize));
		}

		// Creates a string filled with fillCharacter
		Str(char fillCharacter) {
			fill(fillCharacter);
		}

		// Initializes this string with specified string. 
		// If the length of the string provided is shorter than the length of this string, the remaining characters are filled using fillChar.
		Str(const char* string, char fillChar) {
			size_t countedLength = strlen(string);
			fu16 len = bufferSize;
			if (countedLength < len) {
				len = countedLength;
			}
			memcpy(buffer, string, len);
			for(fu16 charIdx = len; charIdx < bufferSize; charIdx++) {
				buffer[charIdx] = fillChar;
			}

			
		}

		// Repeats the character in the span [start, end>.
		// returns:
		//   0 if success
		//  -1 if end > the size of this Str
		//  -2 if end <= start
		int set(fu16 start, fu16 end, char character) {
#ifndef LIBSTR_UNSAFE
			if (end >= bufferSize) {
				return -1;
			}
			if (end <= start) {
				return -2;
			}
#endif // !LIBSTR_UNSAFE
			for (fu16 i = start; i < end; i++) {
				buffer[i] = character;
			}
			return 0;
		}

		// Fills the Str with the spesified character, and appends a null terminator
		void fill(char character) {
			for (fu16 i = 0; i < bufferSize; i++) {
				buffer[i] = character;
			}
		}

		// Copies characters from the data string to this Str. The string should be null terminated if bounds checking is to be performed.
		// Returns:
		//   0 if successful
		//  -1 if out of bounds
		// startIndex: the index of this Str to start writing.
		// numChars: the amount of characters to copy from data
		// data: the string to copy from
		int set(fu16 startIndex, fu16 numChars, const char* data) {
#ifndef LIBSTR_UNSAFE
			if (startIndex + numChars > bufferSize) {
				return -1;	// String out of bounds
			}
			size_t countedLength = strlen(data);
			if (countedLength < numChars) {
				numChars = countedLength;
			}
#endif // !LIBSTR_UNSAFE
			memcpy(buffer + startIndex, data, numChars);
			return 0;
		}

		// Copies all the characters from otherStr to this Str. 
		// startIndex is the index at which to start copying to in this Str.
		// returns:
		//   0 if success
		//  -1 if the writing is out of bounds
		template<fu16 otherSize>
		int set(fu16 startIndex, const Str<otherSize> otherStr) {
#ifndef LIBSTR_UNSAFE
			if (startIndex + otherSize > bufferSize) {
				return - 1;
			}
#endif // !LIBSTR_UNSAFE
			for (fu16 i = 0; i < otherSize; i++) {
				buffer[startIndex + i] = otherStr.buffer[i];
			}
			return 0;
		}

		// Sets one character at the specified index.
		// Returns:
		//   0 if successful
		//  -1 if out of bounds
		int set(fu16 index, char data) {
#ifndef LIBSTR_UNSAFE
			if (index >= bufferSize) {
				return -1;	// Index out of bounds
			}
#endif // !LIBSTR_UNSAFE // !LIBSTR_UNSAFE
			buffer[index] = data;
			return 0;
		}

		// Writes a unsigned long into this str, and pads the left side with 0s.
		// Examples:
		//     padSet(0, 4, 123) -> "0123"
		//     padSet(0, 3, 123) -> "123"
		// Returns:
		//  0 if success
		// -1 if the number does not fit, i.e. start + numChars is out of bounds
		// -2 data has more digits than numChars
		// Params:
		// numChars:
		//   the amount of characters used.
		// start:
		//   the index in this Str to start writing the unsigned long.
		// data:
		//   the unsigned long to be written
		int padSet(fu16 start, fu16 numChars, unsigned long data) {
			// Calculate number of digits
			fu8 numDigits = 0;
			unsigned long num = data;
			do {
				num = num / 10;
				numDigits++;
			} while (num != 0);

#ifndef LIBSTR_UNSAFE
			if (numDigits > numChars) {
				return -2;	// Number of digits is too large
			}
			if (start + numChars > bufferSize) {
				return -1;	// Out of bounds
			}
#endif // !LIBSTR_UNSAFE
			// Write the padding
			for (fu8 i = 0; i < numChars - numDigits; i++) {
				buffer[start + i] = '0';
			}
			// Write the long from right to left
			for (fu8 i = 0; i < numDigits; i++) {
				// Finding the last character by adding the last digit to the ASCII code of '0'(=48)
				char c = '0' + (data % 10);
				buffer[start + (numChars - 1 - i)] = c;
				// Shift the long to the right, so the second last digit is the last
				data /= 10;
			}

			return 0;
		}

		// Writes a long into this str, and pads the left side with 0s.
		// Examples:
		//     padSet(0, 6, 234) -> "00+234"
		//     padSet(0, 4, -234) -> "-234"
		// Returns:
		//  0 if success
		// -1 if the number does not fit, i.e. start + numChars is out of bounds
		// -2 if data has more digits than numChars
		// Params:
		// numChars:
		//   the amount of characters used.
		// start:
		//   the index in this Str to start writing the unsigned long
		// data:
		//   the unsigned long to be written
		int padSet(fu16 start, fu16 numChars, long data) {
			// Calculate number of digits
			fu8 numDigits = 0;
			unsigned long absData = abs(data);
			unsigned long tempNum = absData;
			do {
				tempNum = tempNum / 10;
				numDigits++;
			} while (tempNum != 0);

#ifndef LIBSTR_UNSAFE
			if (numDigits + 1 > numChars) {	 // Adding 1 for the +/-
				return -2;					 // Number of digits is too large
			}
			if (start + numChars > bufferSize) {
				return -1;	// Out of bounds
			}
#endif // !LIBSTR_UNSAFE
			// Write the padding
			for (fu8 i = 0; i < numChars - numDigits; i++) {
				buffer[start + i] = '0';
			}
			if (data > 0) {
				buffer[start + numChars - (numDigits + 1)] = '+';
			}
			else {
				buffer[start + numChars - (numDigits + 1)] = '-';
			}
			// Write the long from right to left
			for (fu8 i = 0; i < numDigits; i++) {
				// Finding the last character by adding the last digit to the ASCII code of '0'(=48)
				char c = '0' + (absData % 10);
				buffer[start + (numChars - 1 - i)] = c;
				// Shift the long to the right, so the second last digit is the last
				absData /= 10;
			}
			return 0;
		}

		// Writes a float with padding. 
		// For example:
		// padSet(0, 7, 1, -12.46) -> "00-12.4"
		// padSet(0, 7, 2, 12.46 -> "0+12.46"
		// Params:
		//  - startIndex : the index to start writing at
		//  - numChars : the total number of characters to be written this includes the decimal point and +/-
		//  - numDecimals : the amount of decimals to write.
		//  - data : the float to write
		int padSet(fu16 startIndex, fu16 numChars, fu16 numDecimals, float data) {
#ifndef LIBSTR_UNSAFE
			if (startIndex + numChars > bufferSize) {
				return -1;
			}
#endif // !LIBSTR_UNSAFE
			// Count digits in the integer part
			long integer = (long)abs(data);
			unsigned int numDigits = 0;
			do {
				integer = integer / 10;
				numDigits++;
			} while (integer != 0);

#ifndef LIBSTR_UNSAFE
			if (numDigits + numDecimals + 2 > numChars) { // +2 = - or + and .
				return -2;
			}
#endif // !LIBSTR_UNSAFE

			fu16 paddingLength = numChars - (2 + numDecimals + numDigits);

			for (fu16 i = 0; i < paddingLength; i++) {
				buffer[startIndex + i] = '0';
			}

			if (data >= 0) {
				buffer[startIndex + paddingLength] = '+';
			}
			else {
				buffer[startIndex + paddingLength] = '-';
				data = -data;
			}

			// Write the integer part
			integer = (long)abs(data);
			for (fu16 i = 0; i < numDigits; i++) {
				buffer[startIndex + paddingLength + 1 + (numDigits-1 - i)] = '0' + (integer % 10);
				integer /= 10;
			}
			
			// Write the dot
			buffer[startIndex + paddingLength + 1 + numDigits] = '.';

			float fractionalPart = data - (long)data;
			for (fu16 i = 0; i < numDecimals; i++) {
				fu8 digit = fractionalPart * 10;
				buffer[startIndex + paddingLength + 1 + numDigits + 1 + i] = '0' + digit;
				
				fractionalPart = fractionalPart * 10;
				fractionalPart = fractionalPart - (long)fractionalPart;
			}
			return 0;
		}

		// Returns a reference to the specified char in this string. If index is negative the access is in reverse, starting at the last element (-1).
		// If not LIBSTR_UNSAFE is set indices out of bounds will be clamped, otherwise accessing out of bounds is undefined behaviour.
		char& operator[](int index) {
			if (index < 0) {
#ifndef LIBSTR_UNSAFE
				if (index < -bufferSize) {
					return buffer[0];
				}
#endif // !LIBSTR_UNSAFE
				index = bufferSize + index;
			}
#ifndef LIBSTR_UNSAFE
			if (index >= bufferSize) {
				return buffer[bufferSize - 1];
			}
#endif // !LIBSTR_UNSAFE
			return buffer[index];
		}
	
		// Returns a copy of the specified char in this string. If index is negative the access is in reverse, starting at the last element (-1).
		// If not LIBSTR_UNSAFE is set indices out of bounds will be clamped, otherwise accessing out of bounds is undefined behaviour.
		char operator[](int index) const {
			if (index < 0) {
#ifndef LIBSTR_UNSAFE
				if (index < -bufferSize) {
					return buffer[0];
				}
#endif // !LIBSTR_UNSAFE
				index = bufferSize + index;
			}
#ifndef LIBSTR_UNSAFE
			if (index >= bufferSize) {
				return buffer[bufferSize - 1];
			}
#endif // !LIBSTR_UNSAFE
			return buffer[index];
		}

	};

	// Concatenates two strings with the addition symbol.
	// Returns the concatenated string.
	template<fu16 S1, fu16 S2>
	Str<S1 + S2> operator+(const Str<S1> left, const Str<S2> right) {
		Str<S1 + S2> str;
		str.set(0, left);
		str.set(S1, right);
		return str;
	}
}