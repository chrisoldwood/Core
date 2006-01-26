////////////////////////////////////////////////////////////////////////////////
//! \author Chris Oldwood
//! \file   COMMON.HPP
//! \brief  File to include the most commonly used headers.

// Check for previous inclusion
#ifndef COMMON_HPP
#define COMMON_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Build control.

#include <Core/Pragmas.hpp>			// Default pragmas.

////////////////////////////////////////////////////////////////////////////////
// Standard headers that conflict with <crtdbg.h> because it remaps malloc,
// calloc, free etc. using #defines.

#include <stdlib.h>
#include <malloc.h>

////////////////////////////////////////////////////////////////////////////////
// Debugging macros and functions.

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC			//!< Enables debug versions of malloc etc.
#include <crtdbg.h>					// CRT debug macros and functions.
#endif

////////////////////////////////////////////////////////////////////////////////
// Library headers.

#include <Core/nullptr.hpp>			// Simulated nullptr proposed keyword.
#include <Core/Types.hpp>			// Typedefs for basic types.
#include <Core/tstring.hpp>			// ANSI/UNICODE string mapping #defines.
#include <Core/SharedPtr.hpp>		// Reference counted pointer class.

#endif // COMMON_HPP
