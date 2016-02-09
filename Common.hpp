////////////////////////////////////////////////////////////////////////////////
//! \file   Common.hpp
//! \brief  File to include the most commonly used headers.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_COMMON_HPP
#define CORE_COMMON_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Library documentation
//
//! \namespace Core
//! \brief     The Core Library namespace.
//!
//! \mainpage  Core C++ Library
//! \section   introduction Introduction
//! This is the base class library for all applications. It provides the basic
//! services such as string handling, smart pointers and debugging utility
//! functions. It also includes a simple unit testing framework.

////////////////////////////////////////////////////////////////////////////////
// Build control.

#include <Core/BuildConfig.hpp>		// Build configuration.
#include <Core/WinTargets.hpp>		// Windows versions.
#include <Core/Pragmas.hpp>			// Default pragmas.

////////////////////////////////////////////////////////////////////////////////
// Compiler provided debugging macros and functions.

#ifdef _DEBUG

#ifdef _MSC_VER

// Standard headers that conflict with <crtdbg.h> because it remaps malloc,
// calloc, free etc. using #defines.
#include <stdlib.h>
#include <malloc.h>
#include <locale>

// CRT debug macros and functions.
#include <crtdbg.h>
#define CORE_CRTDBG_ENABLED		//!< Flag CRT debug functions as available.

// Common STL headers that use the 'new' keyword which is later remapped.
#include <map>

#endif // _MSC_VER

#endif // _DEBUG

////////////////////////////////////////////////////////////////////////////////
// Common STL headers that generate level 4 warnings.

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4702)

#include <vector>
#include <list>

#pragma warning(pop)
#endif

////////////////////////////////////////////////////////////////////////////////
// Library headers.

#include <Core/nullptr.hpp>			// Simulated nullptr proposed keyword.
#include <Core/Types.hpp>			// Typedefs for basic types.
#include <Core/tstring.hpp>			// ANSI/UNICODE string mapping #defines.
#include <Core/Debug.hpp>			// Debugging macros.
#include <Core/NotCopyable.hpp>		// Base class to disable copying.
#include <Core/SmartPtr.hpp>		// Smart-pointer base class.
#include <Core/SharedPtr.hpp>		// Reference counted smart-pointer class.

#endif // CORE_COMMON_HPP
