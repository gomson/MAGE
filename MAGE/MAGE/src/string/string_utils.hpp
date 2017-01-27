#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "support.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	/**
	 Checks whether the given string are equal.

	 @param[in]		str1
					A pointer to the string to be scanned.
	 @param[in]		str2
					A pointer to the string containing the sequence of characters to match.
	 @return		@c true if @a str1 is equal to @a str2.
					@c false otherwise.
	 */
	inline bool str_equals(const char *str1, const char *str2) {
		return strcmp(str1, str2) ? false : true;
	}

	/**
	 Checks whether the first given string contains the second given string.

	 @param[in]		str1
					A pointer to the string to be scanned.
	 @param[in]		str2
					A pointer to the string containing the sequence of characters to match.
	 @return		@c true if @a str1 contains a substring @a str2.
					@c false otherwise.
	 */
	inline bool str_contains(const char *str1, const char *str2) {
		return strstr(str1, str2) ? true : false;
	}

	/**
	 Reads characters from the given input string and stores them as a C string into str 
	 until (num-1) characters have been read or either a newline or the end-of-file is reached, 
	 whichever happens first.

	 A newline character makes @c sgets stop reading, but it is considered a valid character 
	 by the function and included in the string copied to @a str.

	 A terminating null character is automatically appended after the characters copied to @a str.
	 
	 @param[in]		str
					A pointer to an array of chars where the string read is copied.
	 @param[in]		num
					Maximum number of characters to be copied into str 
					(including the terminating null-character).
	 @param[in]		input
					A pointer to to a pointer to an array of chars that represents
					the input string.
	 @note			The @c sgets function is the string variant of @c fgets.
	 */
	char *sgets(char *str, int num, const char **input);
}