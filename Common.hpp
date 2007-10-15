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

////////////////////////////////////////////////////////////////////////////////
// Build control.

#include <Core/BuildConfig.hpp>		// Build configuation.
#include <Core/Pragmas.hpp>			// Default pragmas.

////////////////////////////////////////////////////////////////////////////////
// Standard headers that conflict with <crtdbg.h> because it remaps malloc,
// calloc, free etc. using #defines.

#include <stdlib.h>
#include <malloc.h>

////////////////////////////////////////////////////////////////////////////////
// Compiler provided debugging macros and functions.

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC			//!< Enables debug versions of malloc etc.
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
#include <Core/Debug.hpp>			// Debugging marocs.
#include <Core/tstring.hpp>			// ANSI/UNICODE string mapping #defines.
#include <Core/NotCopyable.hpp>		// Base class to disable copying.
#include <Core/SmartPtr.hpp>		// Smart-pointer base class.
#include <Core/SharedPtr.hpp>		// Reference counted smart-pointer class.

#endif // CORE_COMMON_HPP
