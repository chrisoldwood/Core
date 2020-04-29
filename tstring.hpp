////////////////////////////////////////////////////////////////////////////////
//! \file   tstring.hpp
//! \brief  Build independent string definitions.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_TSTRING_HPP
#define CORE_TSTRING_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Standard headers.

#ifdef _MSC_VER
#pragma push_macro("new")
#undef new
#endif

// Includes internal headers that redefine 'new' .
#include <streambuf>

#ifdef _MSC_VER
#pragma pop_macro("new")
#endif

#include <string>
#include <string.h>

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Calculate the number of bytes required to represent a string of the given
//! number of characters.

template<typename CharT>
inline size_t numBytes(size_t numChars)
{
	return (numChars * sizeof(CharT));
}

////////////////////////////////////////////////////////////////////////////////
//! Calculate the number of bytes required to represent the string.

inline size_t numBytes(const char* string)
{
	return numBytes<char>(strlen(string));
}

////////////////////////////////////////////////////////////////////////////////
//! Calculate the number of bytes required to represent the string.

inline size_t numBytes(const wchar_t* string)
{
	return numBytes<wchar_t>(wcslen(string));
}

//namespace Core
}

////////////////////////////////////////////////////////////////////////////////
// The following #defines mirror the Windows style TCHAR definitions. These
// allow the same codebase to support both ANSI and UNICODE builds. They use a
// consistent 't' prefix for both the types and functions rather than the _tcs
// prefix used by the Windows header.

//
// ANSI build.
//
#ifdef ANSI_BUILD

// Basic types.
typedef char			tchar;		//!< Build agnostic character type. Maps to char or wchat_t.
typedef unsigned char	utchar;		//!< Build agnostic unsigned character type.
#define TXT(x)			x			//!< Build agnostic string literal. Maps to "" or L"".

typedef	std::string		tstring;	//!< Build agnostic string type.
typedef std::ostream	tostream;	//!< Build agnostic output stream type.
typedef std::istream	tistream;	//!< Build agnostic input stream type.

// String functions.
#define tstrlen			strlen
#define tstrnlen		strnlen
#define tstrlwr			_strlwr
#define tstrupr			_strupr
#define tstrcmp			strcmp
#ifdef __BORLANDC__
#define tstricmp		stricmp
#else
#define tstricmp		_stricmp
#endif
#define tstrncmp		strncmp
#ifdef __BORLANDC__
#define tstrnicmp		strnicmp
#else
#define tstrnicmp		_strnicmp
#endif
#define tstrcpy			strcpy
#define tstrncpy		strncpy
#define tstrcat			strcat
#define tstrtok			strtok
#define tstrchr			strchr
#define	tstrrchr		strrchr
#define	tstrstr			strstr
#define tstrtol			strtol
#define tstrtoul		strtoul
#define tstrtod			strtod
#ifdef _MSC_VER
#define tstrtoll		_strtoi64
#define tstrtoull		_strtoui64
#else
#define tstrtoll		strtoll
#define tstrtoull		strtoull
#endif
#define tatoi			atoi
#define tatol			atol
#define tisspace		isspace
#define ttoupper		toupper
#define ttolower		tolower

//
// UNICODE build.
//
#else

// Basic types.
typedef wchar_t			tchar;		//!< Build agnostic character type. Maps to char or wchat_t.
typedef wchar_t			utchar;		//!< Build agnostic unsigned character type.
#define TXT(x)			L ## x		//!< Build agnostic string literal. Maps to "" or L"".

typedef	std::wstring	tstring;	//!< Build agnostic string type.
typedef std::wostream	tostream;	//!< Build agnostic output stream type.
typedef std::wistream	tistream;	//!< Build agnostic input stream type.

// String functions.
#define tstrlen			wcslen
#define tstrnlen		wcsnlen
#define tstrlwr			_wcslwr
#define tstrupr			_wcsupr
#define tstrcmp			wcscmp
#define tstricmp		_wcsicmp
#define tstrncmp		wcsncmp
#define tstrnicmp		_wcsnicmp
#define tstrcpy			wcscpy
#define tstrncpy		wcsncpy
#define tstrcat			wcscat
#define tstrtok			wcstok
#define tstrchr			wcschr
#define	tstrrchr		wcsrchr
#define	tstrstr			wcsstr
#define tstrtol			wcstol
#define tstrtoul		wcstoul
#define tstrtod			wcstod
#ifdef _MSC_VER
#define tstrtoll		_wcstoi64
#define tstrtoull		_wcstoui64
#else
#define tstrtoll		_wcstoll
#define tstrtoull		_wcstoull
#endif
#define tatoi			_wtoi
#define tatol			_wtol
#define tisspace		iswspace
#define ttoupper		towupper
#define ttolower		towlower

#endif

#endif // CORE_TSTRING_HPP
