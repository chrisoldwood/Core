////////////////////////////////////////////////////////////////////////////////
//! \file   StringUtils.cpp
//! \brief  .
//! \author Chris Oldwood

#include "Common.hpp"
#include "StringUtils.hpp"
#include <stdarg.h>
#include <tchar.h>
#include <limits>

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Format the string ala printf. This function is used internally as the
//! underlying function used for all var args string formatting.

std::tstring FmtEx(const tchar* pszFormat, va_list args)
{
	// Allocate the buffer.
	int nLength = _vsctprintf(pszFormat, args);

	std::tstring str(nLength, '\0');

	// Format the string.
	int nResult = _vsntprintf(&str[0], nLength, pszFormat, args);

	ASSERT(nResult == nLength);

	// Check for buffer overrun.
	if (nResult < 0)
		throw std::logic_error(Fmt("Insufficient buffer size calculated in Fmt(). Result: %d", nResult));

	return str;
}

////////////////////////////////////////////////////////////////////////////////
//! Format the string ala printf.

std::tstring Fmt(const tchar* pszFormat, ...)
{
	va_list	args;

	va_start(args, pszFormat);

	return FmtEx(pszFormat, args);
}

//namespace Core
}
