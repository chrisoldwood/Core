////////////////////////////////////////////////////////////////////////////////
//! \file   tstring.hpp
//! \brief  Build independent string defintions.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_TSTRING_HPP
#define CORE_TSTRING_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Standard headers.

#include <string>

////////////////////////////////////////////////////////////////////////////////
// The following #defines mirror the Windows style TCHAR definitions. These
// allow the same codebase to support both ANSI and UNICODE builds. They use a
// consistent 't' prefix for both the types and functions rather than the _tcs
// prefix used by the Windows header.

// ANSI build.
#ifndef _UNICODE

// Basic types.
typedef char			tchar;		//!< Build agnostic character type. Maps to char or wchat_t.
#define TXT(x)			x			//!< Build agnostic string literal. Maps to "" or L"".

namespace std
{
typedef	std::string		tstring;	//!< Build agnostic string type. Maps to string or wstring.
}

// String functions.

// UNICODE build.
#else

// Basic types.
typedef wchar_t			tchar;		//!< Build agnostic character type. Maps to char or wchat_t.
#define TXT(x)			L ## x		//!< Build agnostic string literal. Maps to "" or L"".

namespace std
{
typedef	std::wstring	tstring;	//!< Build agnostic string type. Maps to string or wstring.
}

// String functions.

#endif

#endif // CORE_TSTRING_HPP
