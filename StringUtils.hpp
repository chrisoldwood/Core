////////////////////////////////////////////////////////////////////////////////
//! \file   StringUtils.hpp
//! \brief  Utility functions for formatting strings.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_STRINGUTILS_HPP
#define CORE_STRINGUTILS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <stdarg.h>

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
// Format the string ala printf.

tstring fmtEx(const tchar* format, va_list args);

////////////////////////////////////////////////////////////////////////////////
// Format the string ala printf.

tstring fmt(const tchar* format, ...);

////////////////////////////////////////////////////////////////////////////////
//! Skip any leading white-space.

template<typename First, typename Last>
First skipWhitespace(First first, Last last)
{
	while ( (first != last) && ((*first == TXT(' ')) || (*first == TXT('\t')) || (*first == TXT('\r')) || (*first == TXT('\n'))) )
		++first;

	return first;
}

////////////////////////////////////////////////////////////////////////////////
//! Format a value into a string.

template<typename T>
tstring format(const T& value);

////////////////////////////////////////////////////////////////////////////////
// Format a boolean value into a string.

template<>
tstring format(const bool& value);

////////////////////////////////////////////////////////////////////////////////
// Format a signed integer value into a string.

template<>
tstring format(const int& value);

////////////////////////////////////////////////////////////////////////////////
// Format an unsigned integer value into a string.

template<>
tstring format(const uint& value);

////////////////////////////////////////////////////////////////////////////////
// Format a signed long integer value into a string.

template<>
tstring format(const long& value);

////////////////////////////////////////////////////////////////////////////////
// Format a unsigned long integer value into a string.

template<>
tstring format(const ulong& value);

////////////////////////////////////////////////////////////////////////////////
// Format a signed long long integer value into a string.

template<>
tstring format(const longlong& value);

////////////////////////////////////////////////////////////////////////////////
// Format a unsigned long long integer value into a string.

template<>
tstring format(const ulonglong& value);

////////////////////////////////////////////////////////////////////////////////
// Parse a value from a string.

template<typename T>
T parse(const tstring& buffer); // throw(ParseException)

////////////////////////////////////////////////////////////////////////////////
// Parse a boolean value from a string.

template<>
bool parse(const tstring& buffer); // throw(ParseException)

////////////////////////////////////////////////////////////////////////////////
// Parse a signed integer value from a string.

template<>
int parse(const tstring& buffer); // throw(ParseException)

////////////////////////////////////////////////////////////////////////////////
// Parse an unsigned integer value from a string.

template<>
uint parse(const tstring& buffer); // throw(ParseException)

////////////////////////////////////////////////////////////////////////////////
// Parse a signed long integer value from a string.

template<>
long parse(const tstring& buffer); // throw(ParseException)

////////////////////////////////////////////////////////////////////////////////
// Parse an unsigned long integer value from a string.

template<>
ulong parse(const tstring& buffer); // throw(ParseException)

////////////////////////////////////////////////////////////////////////////////
// Parse a signed long long integer value from a string.

template<>
longlong parse(const tstring& buffer); // throw(ParseException)

////////////////////////////////////////////////////////////////////////////////
// Parse a unsigned long long integer value from a string.

template<>
ulonglong parse(const tstring& buffer); // throw(ParseException)

////////////////////////////////////////////////////////////////////////////////
// Convert a string to upper case.

void makeUpper(tstring& string);

////////////////////////////////////////////////////////////////////////////////
// Create an upper case version of a string.

tstring createUpper(tstring string);

////////////////////////////////////////////////////////////////////////////////
// Convert a string to lower case.

void makeLower(tstring& string);

////////////////////////////////////////////////////////////////////////////////
// Create a lower case version of a string.

tstring createLower(tstring string);

////////////////////////////////////////////////////////////////////////////////
// Trim any leading white-space from the string.

void trimLeft(tstring& string);

////////////////////////////////////////////////////////////////////////////////
// Trim any trailing white-space from the string.

void trimRight(tstring& string);

////////////////////////////////////////////////////////////////////////////////
// Trim any leading or trailing white-space from the string.

void trim(tstring& string);

////////////////////////////////////////////////////////////////////////////////
// Trim any leading or trailing white-space from a copy of the string.

tstring trimCopy(tstring string);

////////////////////////////////////////////////////////////////////////////////
// Extract the leftmost N characters as a sub-string.

tstring left(const tstring& string, size_t count);

////////////////////////////////////////////////////////////////////////////////
// Extract the rightmost N characters as a sub-string.

tstring right(const tstring& string, size_t count);

////////////////////////////////////////////////////////////////////////////////
// Case-insensitive version of strstr().

const tchar* tstristr(const tchar* string, const tchar* search);

////////////////////////////////////////////////////////////////////////////////
// Case-insensitive version of strstr().

inline const tchar* tstristr(const tstring& string, const tstring& search)
{
	return tstristr(string.c_str(), search.c_str());
}

////////////////////////////////////////////////////////////////////////////////
// Replace all occurrences of a pattern in a string with another.

tstring replace(const tstring& string, const tstring& pattern, const tstring& replacement);

////////////////////////////////////////////////////////////////////////////////
// Replace all occurrences of a (case-insensitive) pattern in a string with
// another.

tstring replaceNoCase(const tstring& string, const tstring& pattern, const tstring& replacement);

//namespace Core
}

// Only available from VC++ 8.0, 64-bit TDM-GCC or MinGW 6.0.
#if (defined(_MSC_VER) && (_MSC_VER < 1400)) || (defined(__GNUC__) && (!defined(_WIN64) && (__GNUC__ < 6)))

////////////////////////////////////////////////////////////////////////////////
// Get the length of a string, using the current locale or one that has been
// passed in. More secure versions of strlen().

size_t strnlen(const char* string, size_t bufsize);
size_t wcsnlen(const wchar_t* string, size_t bufsize);

#endif

#endif // CORE_STRINGUTILS_HPP
