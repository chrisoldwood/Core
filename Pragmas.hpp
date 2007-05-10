////////////////////////////////////////////////////////////////////////////////
//! \file   Pragmas.hpp
//! \brief  Disables annoying Level 4 warnings.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_PRAGMAS_HPP
#define CORE_PRAGMAS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

// Nameless struct/union.
#pragma warning ( disable : 4201 )

// 'this' used in base member initialisation list.
#pragma warning ( disable : 4355 )

// Unreferenced local function has been removed.
#pragma warning ( disable : 4505 )

// Copy constructor could not be generated.
// Caused by members which are references.
#pragma warning ( disable : 4511 )

// No assignment operator could be generated.
// Caused by members which are references.
#pragma warning ( disable : 4512 )

// Unreferenced inline function removed.
#pragma warning ( disable : 4514 )

// Creating/using precompiled header.
#pragma warning ( disable : 4699 )

// Statement has no effect
//#pragma warning ( disable : 4705 )

// Inline function not expanded.
#pragma warning ( disable : 4710 )

// Forcing value to bool true or false.
#pragma warning ( disable : 4800 )

#ifdef _DEBUG

// Unreachable code.
// Caused by ASSERT macros when compiling a release build with asserts.
#pragma warning ( disable : 4702 )

#endif

#endif // CORE_PRAGMAS_HPP
