////////////////////////////////////////////////////////////////////////////////
//! \file   StringUtils.cpp
//! \brief  Utility functions for formatting strings.
//! \author Chris Oldwood

#include "Common.hpp"
#include "StringUtils.hpp"
#include <stdarg.h>
#include <tchar.h>
#include <limits>
#include <stdio.h>
#include <Core/BadLogicException.hpp>
#include <Core/ParseException.hpp>
#include "AnsiWide.hpp"
#include <locale>
#include <stdlib.h>

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Format the string ala printf. This function is used internally as the
//! underlying function used for all var args string formatting.

tstring fmtEx(const tchar* format, va_list args)
{
#ifdef _MSC_VER

	// Allocate the buffer.
	size_t length = _vsctprintf(format, args);

	tstring str(length, TXT('\0'));

	// Format the string.
	int result = _vsntprintf(&str[0], length, format, args);

	ASSERT(static_cast<size_t>(result) == length);

	// Handle any errors.
	if (result < 0)
		throw BadLogicException(fmt(TXT("Invalid format or buffer size used in Fmt(). Result: %d"), result));

	return str;

#else

	// Allocate an initial buffer.
	size_t length = 256;

	tstring str(length, TXT('\0'));

	int result = -1;

	// Format the string, growing the buffer and repeating if necessary.
	while ((result = _vsntprintf(&str[0], length, format, args)) == -1)
	{
		length *= 2;
		str.resize(length, TXT('\0'));
	}

	ASSERT(static_cast<size_t>(result) <= length);

	// Handle any errors.
	if (result < 0)
		throw BadLogicException(fmt(TXT("Invalid format or buffer size used in Fmt(). Result: %d"), result));

	str.resize(result);

	return str;

#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Format the string ala printf.

tstring fmt(const tchar* format, ...)
{
	va_list	args;

	va_start(args, format);

	return fmtEx(format, args);
}

////////////////////////////////////////////////////////////////////////////////
//! Traits to invoke the underlying formatting and parsing functions for the
//! specified template type.

template<typename T>
struct FormatTraits
{};

////////////////////////////////////////////////////////////////////////////////
//! Formatting and parsing functions for handling int's.

template<>
struct FormatTraits<int>
{
	static int format(tchar* buffer, size_t size, int value)
	{
		return _sntprintf(buffer, size, TXT("%d"), value);
	}

	static long parse(const tchar* nptr, tchar** endptr, int base)
	{
		return tstrtol(nptr, endptr, base);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! Formatting and parsing functions for handling unsigned int's.

template<>
struct FormatTraits<uint>
{
	static int format(tchar* buffer, size_t size, uint value)
	{
		return _sntprintf(buffer, size, TXT("%u"), value);
	}

	static ulong parse(const tchar* nptr, tchar** endptr, int base)
	{
		if (*nptr == TXT('-'))
		{
			errno = EINVAL;
			return 0;
		}

		return tstrtoul(nptr, endptr, base);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! Formatting and parsing functions for handling long long int's.

template<>
struct FormatTraits<longlong>
{
	static int format(tchar* buffer, size_t size, longlong value)
	{
		return _sntprintf(buffer, size, TXT("%I64d"), value);
	}

	static longlong parse(const tchar* nptr, tchar** endptr, int base)
	{
		return tstrtoll(nptr, endptr, base);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! Formatting and parsing functions for handling unsigned long long int's.

template<>
struct FormatTraits<ulonglong>
{
	static int format(tchar* buffer, size_t size, ulonglong value)
	{
		return _sntprintf(buffer, size, TXT("%I64u"), value);
	}

	static ulonglong parse(const tchar* nptr, tchar** endptr, int base)
	{
		if (*nptr == TXT('-'))
		{
			errno = EINVAL;
			return 0;
		}

		return tstrtoull(nptr, endptr, base);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! Generic function for parsing integers.

template<typename T, typename Traits>
tstring formatInteger(const T& value)
{
	tchar buffer[std::numeric_limits<T>::digits10+3]; // '+-' + (digits+1) + '\0'

	int result = Traits::format(buffer, ARRAY_SIZE(buffer), value);

	if (result < 0)
		throw BadLogicException(Core::fmt(TXT("Insufficient sized buffer passed in format<>(). Result: %d"), result));

	return buffer;
}

////////////////////////////////////////////////////////////////////////////////
//! Format a boolean value into a string.

template<>
tstring format(const bool& value)
{
	return (value) ? TXT("1") : TXT("0");
}

////////////////////////////////////////////////////////////////////////////////
//! Format a signed integer value into a string.

template<>
tstring format(const int& value)
{
	return formatInteger< int, FormatTraits<int> >(value);
}

////////////////////////////////////////////////////////////////////////////////
//! Format an unsigned integer value into a string.

template<>
tstring format(const uint& value)
{
	return formatInteger< uint, FormatTraits<uint> >(value);
}

////////////////////////////////////////////////////////////////////////////////
//! Format a signed long long integer value into a string.

template<>
tstring format(const longlong& value)
{
	return formatInteger< longlong, FormatTraits<longlong> >(value);
}

////////////////////////////////////////////////////////////////////////////////
//! Format a unsigned long long integer value into a string.

template<>
tstring format(const ulonglong& value)
{
	return formatInteger< ulonglong, FormatTraits<ulonglong> >(value);
}

////////////////////////////////////////////////////////////////////////////////
//! Generic function for parsing integers.

template<typename T, typename Traits>
T parseInteger(const tstring& buffer)
{
	// Get buffer iterators.
	const tchar* it     = buffer.c_str();
	const tchar* end    = it + buffer.length();
	tchar*       endPtr = nullptr;

	// Skip leading whitespace.
	it = skipWhitespace(it, end);

	if (it == end)
		throw ParseException(Core::fmt(TXT("Failed to parse number: '%s'"), buffer.c_str()));

	errno = 0;

	// Parse value.
	T value = Traits::parse(it, &endPtr, 10);

	if ( (endPtr == nullptr) || (errno != 0) )
		throw ParseException(Core::fmt(TXT("Failed to parse number: '%s'"), buffer.c_str()));

	it = endPtr;

	// Skip trailing whitespace.
	it = skipWhitespace(it, end);

	if ( (it != end) && (*it != TXT('\0')) )
		throw ParseException(Core::fmt(TXT("Failed to parse number: '%s'"), buffer.c_str()));

	return value;
}

////////////////////////////////////////////////////////////////////////////////
//! Parse a boolean value from a string.

template<>
bool parse(const tstring& buffer)
{
	uint value = parseInteger< uint, FormatTraits<uint> >(buffer);

	if ( (value != 0) && (value != 1) )
		throw ParseException(Core::fmt(TXT("Failed to parse boolean value: '%s'"), buffer.c_str()));

	return (value == 1);
}

////////////////////////////////////////////////////////////////////////////////
// Parse a signed integer value from a string.

template<>
int parse(const tstring& buffer)
{
	return parseInteger< int, FormatTraits<int> >(buffer);
}

////////////////////////////////////////////////////////////////////////////////
// Parse an unsigned integer value from a string.

template<>
uint parse(const tstring& buffer)
{
	return parseInteger< uint, FormatTraits<uint> >(buffer);
}

////////////////////////////////////////////////////////////////////////////////
// Parse a signed long long integer value from a string.

template<>
longlong parse(const tstring& buffer)
{
	return parseInteger< longlong, FormatTraits<longlong> >(buffer);
}

////////////////////////////////////////////////////////////////////////////////
// Parse a unsigned long long integer value from a string.

template<>
ulonglong parse(const tstring& buffer)
{
	return parseInteger< ulonglong, FormatTraits<ulonglong> >(buffer);
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string to upper case.

void makeUpper(tstring& string)
{
	if (string.empty())
		return;

	std::locale  loc;
	tchar*       begin = &string[0];
	const tchar* end   = begin + string.length();

	std::use_facet< std::ctype<tchar> >(loc).toupper(begin, end);
}

////////////////////////////////////////////////////////////////////////////////
//! Create an upper case version of a string.

tstring createUpper(tstring string)
{
	makeUpper(string);
	return string;
}

////////////////////////////////////////////////////////////////////////////////
//! Convert a string to lower case.

void makeLower(tstring& string)
{
	if (string.empty())
		return;

	std::locale  loc;
	tchar*       begin = &string[0];
	const tchar* end   = begin + string.length();

	std::use_facet< std::ctype<tchar> >(loc).tolower(begin, end);
}

////////////////////////////////////////////////////////////////////////////////
//! Create a lower case version of a string.

tstring createLower(tstring string)
{
	makeLower(string);
	return string;
}

////////////////////////////////////////////////////////////////////////////////
//! Trim any leading whitespace from the string.

void trimLeft(tstring& string)
{
	tstring::iterator it = skipWhitespace(string.begin(), string.end());

	if (it != string.begin())
		string.erase(string.begin(), it);
}

////////////////////////////////////////////////////////////////////////////////
//! Trim any trailing whitespace from the string.

void trimRight(tstring& string)
{
	tstring::reverse_iterator it = skipWhitespace(string.rbegin(), string.rend());

	if (it != string.rbegin())
		string.erase(it.base(), string.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Trim any leading or trailing whitespace from the string.

void trim(tstring& string)
{
	trimLeft(string);
	trimRight(string);
}

////////////////////////////////////////////////////////////////////////////////
//! Trim any leading or trailing whitespace from a copy of the string.

tstring trimCopy(tstring string)
{
	trim(string);
	return string;
}

////////////////////////////////////////////////////////////////////////////////
//! Extract the leftmost N characters as a substring.

tstring left(const tstring& string, size_t count)
{
	return string.substr(0, count);
}

////////////////////////////////////////////////////////////////////////////////
//! Extract the rightmost N characters as a substring.

tstring right(const tstring& string, size_t count)
{
	const size_t length = string.length();
	const size_t offset = (count >= length) ? 0 : (length - count);

	return string.substr(offset, count);
}

//namespace Core
}

// Only available from VC++ 8.0
#if _MSC_VER < 1400

////////////////////////////////////////////////////////////////////////////////
//! Get the length of a string, using the current locale or one that has been
//! passed in. More secure versions of strlen().

size_t strnlen(const char* string, size_t bufsize)
{
	size_t count = 0;
	const char* end = string+bufsize;

	for (const char* it = string; ((it != end) && (*it != '\0')); ++it)
		++count;

	return count;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the length of a string, using the current locale or one that has been
//! passed in. More secure versions of strlen().

size_t wcsnlen(const wchar_t* string, size_t bufsize)
{
	size_t count = 0;
	const wchar_t* end = string+bufsize;

	for (const wchar_t* it = string; ((it != end) && (*it != L'\0')); ++it)
		++count;

	return count;
}

#endif
