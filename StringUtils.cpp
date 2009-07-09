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

tstring FmtEx(const tchar* pszFormat, va_list args)
{
#ifdef _MSC_VER

	// Allocate the buffer.
	size_t nLength = _vsctprintf(pszFormat, args);

	tstring str(nLength, TXT('\0'));

	// Format the string.
	int nResult = _vsntprintf(&str[0], nLength, pszFormat, args);

	ASSERT(static_cast<size_t>(nResult) == nLength);

	// Handle any errors.
	if (nResult < 0)
		throw BadLogicException(Fmt(TXT("Invalid format or buffer size used in Fmt(). Result: %d"), nResult));

	return str;

#else

	// Allocate an initial buffer.
	size_t nLength = 256;

	tstring str(nLength, TXT('\0'));

	int nResult = -1;

	// Format the string, growing the buffer and repeating if necessary.
	while ((nResult = _vsntprintf(&str[0], nLength, pszFormat, args)) == -1)
	{
		nLength *= 2;
		str.resize(nLength, TXT('\0'));
	}

	ASSERT(static_cast<size_t>(nResult) <= nLength);

	// Handle any errors.
	if (nResult < 0)
		throw BadLogicException(Fmt(TXT("Invalid format or buffer size used in Fmt(). Result: %d"), nResult));

	str.resize(nResult);

	return str;

#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Format the string ala printf.

tstring Fmt(const tchar* pszFormat, ...)
{
	va_list	args;

	va_start(args, pszFormat);

	return FmtEx(pszFormat, args);
}

////////////////////////////////////////////////////////////////////////////////
// Traits to invoke the underlying formatting and parsing functions.

template<typename T>
struct FormatTraits
{};

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

template<>
struct FormatTraits<uint>
{
	static int format(tchar* buffer, size_t size, uint value)
	{
		return _sntprintf(buffer, size, TXT("%u"), value);
	}

	static ulong parse(const tchar* nptr, tchar** endptr, int base)
	{
		return tstrtoul(nptr, endptr, base);
	}
};

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

template<>
struct FormatTraits<ulonglong>
{
	static int format(tchar* buffer, size_t size, ulonglong value)
	{
		return _sntprintf(buffer, size, TXT("%I64u"), value);
	}

	static ulonglong parse(const tchar* nptr, tchar** endptr, int base)
	{
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
		throw BadLogicException(Core::Fmt(TXT("Insufficient sized buffer passed in format<>(). Result: %d"), result));

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
		throw ParseException(Core::Fmt(TXT("Invalid number: '%s'"), buffer.c_str()));

	errno = 0;

	// Parse value.
	T value = Traits::parse(it, &endPtr, 10);

	if ( (endPtr == nullptr) || (errno != 0) )
		throw ParseException(Core::Fmt(TXT("Invalid number: '%s'"), buffer.c_str()));

	it = endPtr;

	// Skip trailing whitespace.
	it = skipWhitespace(it, end);

	if ( (it != end) && (*it != TXT('\0')) )
		throw ParseException(Core::Fmt(TXT("Invalid number: '%s'"), buffer.c_str()));

	return value;
}

////////////////////////////////////////////////////////////////////////////////
//! Parse a boolean value from a string.

template<>
bool parse(const tstring& buffer)
{
	uint value = parseInteger< uint, FormatTraits<uint> >(buffer);

	if ( (value != 0) && (value != 1) )
		throw ParseException(Core::Fmt(TXT("Invalid boolean: '%s'"), buffer.c_str()));

	return (value == 1);
}

////////////////////////////////////////////////////////////////////////////////
//! Parse a signed integer value from a string.

template<>
int parse(const tstring& buffer)
{
	return parseInteger< int, FormatTraits<int> >(buffer);
}

////////////////////////////////////////////////////////////////////////////////
//! Parse an unsigned integer value from a string.

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
//! Parse a unsigned long long integer value from a string.

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

tstring createUpper(const tstring& string)
{
	tstring upper(string);

	makeUpper(upper);

	return upper;
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

tstring createLower(const tstring& string)
{
	tstring lower(string);

	makeLower(lower);

	return lower;
}

//namespace Core
}
