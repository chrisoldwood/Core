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

// Visual C++
#ifdef _MSC_VER

////////////////////////////////////////////////////////////////////////////////
// Manually define the intrinsic forms of the _Interlocked*() functions to avoid
// bringing in <windows.h>.

extern "C" long __cdecl _InterlockedIncrement(volatile long* lpValue);
extern "C" long __cdecl _InterlockedDecrement(volatile long* lpValue);

#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Thread-safe function for incrementing the value by one.

inline long atomicIncrement(volatile long& value)
{
	return _InterlockedIncrement(&value);
}

////////////////////////////////////////////////////////////////////////////////
//! Thread-safe function for decrementing the value by one.

inline long atomicDecrement(volatile long& value)
{
	return _InterlockedDecrement(&value);
}

//namespace Core
}

// Not Visual C++
#else

////////////////////////////////////////////////////////////////////////////////
// Manually define the exported Windows SDK Interlocked*() functions to avoid
// bringing in <windows.h>.

#ifndef _WIN64
#if (__GNUC__ >= 4)
extern "C" long __stdcall InterlockedIncrement(volatile long* lpValue);
extern "C" long __stdcall InterlockedDecrement(volatile long* lpValue);
#else
extern "C" long __stdcall InterlockedIncrement(long* lpValue);
extern "C" long __stdcall InterlockedDecrement(long* lpValue);
#endif
#else
#if defined __BORLANDC__
extern "C" long __cdecl InterlockedIncrement(volatile long* lpValue);
extern "C" long __cdecl InterlockedDecrement(volatile long* lpValue);
#else
extern "C" long __cdecl _InterlockedIncrement(volatile long* lpValue);
extern "C" long __cdecl _InterlockedDecrement(volatile long* lpValue);
#define InterlockedIncrement _InterlockedIncrement
#define InterlockedDecrement _InterlockedDecrement
#endif
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Thread-safe function for incrementing the value by one.

inline long atomicIncrement(long& value)
{
	return InterlockedIncrement(&value);
}

////////////////////////////////////////////////////////////////////////////////
//! Thread-safe function for decrementing the value by one.

inline long atomicDecrement(long& value)
{
	return InterlockedDecrement(&value);
}

//namespace Core
}

#endif

#endif // CORE_INTERLOCKED_HPP
