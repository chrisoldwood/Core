////////////////////////////////////////////////////////////////////////////////
//! \file   Debug.hpp
//! \brief  Debug reporting functions and macros.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_DEBUG_HPP
#define CORE_DEBUG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
// Functions available in all builds.

// Enable or disable memory leak reporting.
void enableLeakReporting(bool enable);

// Write a message to the debugger stream in both Debug and Release builds.
void debugWrite(const tchar* format, ...);

////////////////////////////////////////////////////////////////////////////////
// Debug versions

#ifdef _DEBUG

// The function invoked when an ASSERT fails in a Debug build.
void assertFail(const char* expression, const char* file, uint line);

// Function to write a message to the debugger output in a Debug build.
void traceEx(const tchar* format, ...);

//! Evaluate the expression, and complain if 'false'.
#define ASSERT(x)		if (x) {} else Core::assertFail((#x), __FILE__, __LINE__)

//! Always fail.
#define ASSERT_FALSE()	Core::assertFail("FALSE", __FILE__, __LINE__)

// Printf style trace messaging.
#define	TRACE(x)				Core::traceEx(x)
#define TRACE1(x,a)				Core::traceEx(x, a)
#define TRACE2(x,a,b)			Core::traceEx(x, a, b)
#define TRACE3(x,a,b,c)			Core::traceEx(x, a, b, c)
#define TRACE4(x,a,b,c,d)		Core::traceEx(x, a, b, c, d)
#define TRACE5(x,a,b,c,d,e)		Core::traceEx(x, a, b, c, d, e)

//! Map calls to 'new' to the debug version and track the file and line number.
#define DBGCRT_NEW	new(_NORMAL_BLOCK, __FILE__, __LINE__)

#ifdef _MSC_VER
//! Enable tracking of 'new' calls by default.
#define new DBGCRT_NEW
#endif

//! Mark a variable as only used in debug to avoid 'unreferenced variable' warnings.
#define DEBUG_USE_ONLY(x)		((void)x)

//! Mark a variable or parameter as unused in any build.
#define UNUSED_VARIABLE(x)		((void)x)

//! The STATIC_ASSERT base template.
template<bool C> struct STATIC_ASSERT_CHECK;

//! The STATIC_ASSERT template matching valid assertions.
template<> struct STATIC_ASSERT_CHECK<true> {};

//! Compile time ASSERT.
#define STATIC_ASSERT(x)		enum { static_assert##__LINE__ = sizeof(Core::STATIC_ASSERT_CHECK<(x)>) };

//! Debug only expression.
#define ASSERT_RESULT(v, e)		ASSERT(e);			\
								DEBUG_USE_ONLY(v)

////////////////////////////////////////////////////////////////////////////////
// Release versions

#else // NDEBUG

#define ASSERT(x)
#define ASSERT_FALSE()

#define	TRACE(x)
#define TRACE1(x,a)
#define TRACE2(x,a,b)
#define TRACE3(x,a,b,c)
#define TRACE4(x,a,b,c,d)
#define TRACE5(x,a,b,c,d,e)

#define DEBUG_USE_ONLY(x)	((void)x)

#define UNUSED_VARIABLE(x)	((void)x)

#define STATIC_ASSERT(x)

#define ASSERT_RESULT(v, e)	(v)

#endif // _DEBUG

//namespace Core
}

#endif // CORE_DEBUG_HPP
