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
// Standard headers that conflict with <crtdbg.h> because it remaps malloc,
// calloc, free etc. using #defines.

#include <stdlib.h>
#include <malloc.h>
#include <locale>

////////////////////////////////////////////////////////////////////////////////
// Compiler provided debugging macros and functions.

#ifdef _DEBUG
#include <crtdbg.h>					// CRT debug macros and functions.
#endif

////////////////////////////////////////////////////////////////////////////////
// Common STL headers that generate level 4 warnings.

#pragma warning ( push )
#pragma warning ( disable : 4702 )
#include <vector>
#include <list>
#pragma warning ( pop )

////////////////////////////////////////////////////////////////////////////////
// Common STL headers that use the 'new' keyword which is later remamped.

#include <map>

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
