////////////////////////////////////////////////////////////////////////////////
//! \file   tiosfwd.hpp
//! \brief  Build independent iosfwd definitions.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_TIOSFWD_HPP
#define CORE_TIOSFWD_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Standard headers.

#include <iosfwd>

////////////////////////////////////////////////////////////////////////////////
// Basic streams aliases.

//
// ANSI build.
//
#ifdef ANSI_BUILD

typedef std::istream  tistream;
typedef std::ostream  tostream;
typedef std::iostream tiostream;

typedef std::istringstream tistringstream;
typedef std::ostringstream tostringstream;
typedef std::stringstream  tstringstream;

typedef std::ifstream tifstream;
typedef std::ofstream tofstream;
typedef std::fstream  tfstream;

//
// UNICODE build.
//
#else

typedef std::wistream  tistream;
typedef std::wostream  tostream;
typedef std::wiostream tiostream;

typedef std::wistringstream tistringstream;
typedef std::wostringstream tostringstream;
typedef std::wstringstream  tstringstream;

typedef std::wifstream tifstream;
typedef std::wofstream tofstream;
typedef std::wfstream  tfstream;

#endif

#endif // CORE_TIOSFWD_HPP
