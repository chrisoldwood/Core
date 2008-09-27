////////////////////////////////////////////////////////////////////////////////
//! \file   tiostream.hpp
//! \brief  Build independent iostream definitions.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_TIOSTREAM_HPP
#define CORE_TIOSTREAM_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Standard headers.

#include <iostream>

////////////////////////////////////////////////////////////////////////////////
// Standard streams aliases.

//
// ANSI build.
//
#ifdef ANSI_BUILD

using std::cin;
using std::cout;
using std::cerr;

// Standard streams
#define tcin			cin
#define tcout			cout
#define tcerr			cerr

//
// UNICODE build.
//
#else

using std::wcin;
using std::wcout;
using std::wcerr;

// Standard streams
#define tcin			wcin
#define tcout			wcout
#define tcerr			wcerr

#endif

#endif // CORE_TIOSTREAM_HPP
