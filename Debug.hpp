////////////////////////////////////////////////////////////////////////////////
//! \file   Assert.hpp
//! \brief  Debugging macros.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_ASSERT_HPP
#define CORE_ASSERT_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
// Debug versions

#ifdef _DEBUG

// The function invoked when an ASSERT fails in a Debug build.
void AssertFail(const char* pszExpression, const char* pszFile, uint iLine);

// Function to write a message to the debugger output in a Debug build.
void TraceEx(const char* pszFormat, ...);

// Write a message to the debugger stream in both Debug and Release builds.
void DebugWrite(const char* pszFormat, ...);

//! Evaluate the expression, and complain if 'false'.
#define ASSERT(x)		if (x) {} else Core::AssertFail(#x, __FILE__, __LINE__)

//! Always fail.
#define ASSERT_FALSE()	Core::AssertFail("FALSE", __FILE__, __LINE__)

// Printf style trace messaging.
#define	TRACE(x)				Core::TraceEx(x)
#define TRACE1(x,a)				Core::TraceEx(x, a)
#define TRACE2(x,a,b)			Core::TraceEx(x, a, b)
#define TRACE3(x,a,b,c)			Core::TraceEx(x, a, b, c)
#define TRACE4(x,a,b,c,d)		Core::TraceEx(x, a, b, c, d)
#define TRACE5(x,a,b,c,d,e)		Core::TraceEx(x, a, b, c, d, e)

//! Map calls to 'new' to the debug version and track the file and line number.
#define DBGCRT_NEW	new(_NORMAL_BLOCK, __FILE__, __LINE__)

//! Mark a variable as only used in debug to avoid 'unreferenced variable' warnings.
#define DEBUG_USE_ONLY(x)

////////////////////////////////////////////////////////////////////////////////
// Release versions

#else // NDEBUG

//! Write a message to the debugger stream in both Debug and Release builds.
void DebugWrite(const char* pszFormat, ...);

#define ASSERT(x)
#define ASSERT_FALSE()

#define	TRACE(x)
#define TRACE1(x,a)
#define TRACE2(x,a,b)
#define TRACE3(x,a,b,c)
#define TRACE4(x,a,b,c,d)
#define TRACE5(x,a,b,c,d,e)

#define DEBUG_USE_ONLY(x)	(x)

#endif // _DEBUG

//namespace Core
}

#endif // CORE_ASSERT_HPP
