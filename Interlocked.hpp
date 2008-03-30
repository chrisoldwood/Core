////////////////////////////////////////////////////////////////////////////////
//! \file   Interlocked.hpp
//! \brief  Define the intrinsic forms of the Interlocked* functions.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_INTERLOCKED_HPP
#define CORE_INTERLOCKED_HPP

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////
// Manually define the intrinsic forms of the Interlocked*() functions to avoid
// bringing in <windows.h>.

extern "C" long __cdecl _InterlockedIncrement(volatile long *lpValue);
extern "C" long __cdecl _InterlockedDecrement(volatile long *lpValue);

#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)

#endif // CORE_INTERLOCKED_HPP
