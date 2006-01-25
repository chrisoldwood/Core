/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		TSTRING.HPP
** COMPONENT:	Core C++ Library
** DESCRIPTION:	Build independent string defintions.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef TSTRING_HPP
#define TSTRING_HPP

#if _MSC_VER > 1000
#pragma once
#endif

/******************************************************************************
**
** Standard headers.
**
*******************************************************************************
*/

//#include <string>

/******************************************************************************
** 
** The following #defines mirror the Windows style TCHAR definitions. These
** allow the same codebase to support both ANSI and UNICODE builds. They use a
** consistent 't' prefix for both the types and functions rather than the _tcs
** prefix used by the Windows header.
**
*******************************************************************************
*/

// ANSI build.
#ifndef _UNICODE

// Basic types.
typedef char	tchar;
#define tstring	string
#define TXT(x)	x

// String functions.

// UNICODE build.
#else

// Basic types.
typedef wchar_t	tchar;
#define tstring	wstring
#define TXT(x)	L ## x

// String functions.

#endif

#endif // TSTRING_HPP
