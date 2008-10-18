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
#include "AnsiWide.hpp"

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Format the string ala printf. This function is used internally as the
//! underlying function used for all var args string formatting.

tstring FmtEx(const tchar* pszFormat, va_list args)
{
	// Allocate the buffer.
	int nLength = _vsctprintf(pszFormat, args);

	tstring str(nLength, TXT('\0'));

	// Format the string.
	int nResult = _vsntprintf(&str[0], nLength, pszFormat, args);

	ASSERT(nResult == nLength);

	// Check for buffer overrun.
	if (nResult < 0)
		throw BadLogicException(Fmt(TXT("Insufficient buffer size calculated in Fmt(). Result: %d"), nResult));

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
