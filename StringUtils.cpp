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
	// Allocate the buffer.
#ifdef _MSC_VER
	int nLength = _vsctprintf(pszFormat, args);
#else
	int nLength = 1024;
#endif

	tstring str(nLength, TXT('\0'));

	// Format the string.
	int nResult = _vsntprintf(&str[0], nLength, pszFormat, args);

	ASSERT(nResult == nLength);

	// Check for buffer overrun.
	if (nResult < 0)
		throw BadLogicException(Fmt(TXT("Insufficient buffer size calculated in Fmt(). Result: %d"), nResult));

#ifndef _MSC_VER
	str.resize(nResult);
#endif

	return str;
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
	tchar buffer[std::numeric_limits<int>::digits10+3]; // '+-' + (digits+1) + '\0'

	int result = _sntprintf(buffer, ARRAY_SIZE(buffer), TXT("%d"), value);

	if (result < 0)
		throw BadLogicException(Core::Fmt(TXT("Insufficient sized buffer passed in format<int>(). Result: %d"), result));

	return buffer;
}

////////////////////////////////////////////////////////////////////////////////
//! Format an unsigned integer value into a string.

template<>
tstring format(const uint& value)
{
	tchar buffer[std::numeric_limits<uint>::digits10+2]; // (digits+1) + '\0'

	int result = _sntprintf(buffer, ARRAY_SIZE(buffer), TXT("%u"), value);

	if (result < 0)
		throw BadLogicException(Core::Fmt(TXT("Insufficient sized buffer passed in format<uint>(). Result: %d"), result));

	return buffer;
}

////////////////////////////////////////////////////////////////////////////////
// Format a 64-bit unsigned integer value into a string.

template<>
tstring format(const uint64& /*value*/)
{
	ASSERT_FALSE();

	return _T("");
}

////////////////////////////////////////////////////////////////////////////////
//! Parse a boolean value from a string.

template<>
bool parse(const tstring& buffer)
{
	// Get buffer iterators.
	const tchar* it  = buffer.c_str();
	const tchar* end = it + buffer.length();

	// Skip leading whitespace.
	it = skipWhitespace(it, end);

	if (it == end)
		throw ParseException(Core::Fmt(TXT("Invalid boolean: '%s'"), buffer.c_str()));

	bool value;

	// Parse value.
	if (*it == TXT('1'))
	{
		value = true;
		++it;
	}
	else if (*it == TXT('0'))
	{
		value = false;
		++it;
	}
	else
	{
		throw ParseException(Core::Fmt(TXT("Invalid boolean: '%s'"), buffer.c_str()));
	}

	// Skip trailing whitespace.
	it = skipWhitespace(it, end);

	if (it != end)
		throw ParseException(Core::Fmt(TXT("Invalid boolean: '%s'"), buffer.c_str()));

	return value;
}

////////////////////////////////////////////////////////////////////////////////
//! Parse a signed integer value from a string.

template<>
int parse(const tstring& buffer)
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
	uint value = tstrtol(it, &endPtr, 10);

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
//! Parse an unsigned integer value from a string.

template<>
uint parse(const tstring& buffer)
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
	uint value = tstrtoul(it, &endPtr, 10);

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
//! Parse a 64-bit unsigned integer value from a string.

template<>
uint64 parse(const tstring& /*buffer*/)
{
	ASSERT_FALSE();

	return 0;
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
