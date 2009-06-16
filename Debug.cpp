////////////////////////////////////////////////////////////////////////////////
//! \file   Debug.cpp
//! \brief  Debug reporting functions.
//! \author Chris Oldwood

#include "Common.hpp"
#include <stdio.h>
#include <stdarg.h>
#include "StringUtils.hpp"
#include "AnsiWide.hpp"

#if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2)) // GCC 4.2+
// Caused by the UNUSED_VARIABLE macro.
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

////////////////////////////////////////////////////////////////////////////////
// Avoid bringing in <windows.h>.

#ifndef _STLPORT_VERSION
extern "C" void __stdcall OutputDebugStringA(const char*);
#endif
extern "C" void __stdcall OutputDebugStringW(const wchar_t*);

#ifdef ANSI_BUILD
#define	OutputDebugString	OutputDebugStringA
#else
#define	OutputDebugString	OutputDebugStringW
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
// Debug build only functions.

#ifdef _DEBUG

////////////////////////////////////////////////////////////////////////////////
//! The function invoked when an ASSERT fails. It displays a message box with
//! details about why and where the assert failed. If asked it will also cause
//! an INT 3 to kick in the debugger.

void AssertFail(const char* pszExpression, const char* pszFile, uint nLine)
{
#ifdef CORE_CRTDBG_ENABLED
	// Output using CRT function.
	if (_CrtDbgReport(_CRT_ASSERT, pszFile, nLine, NULL, "%s", pszExpression) == 1)
		_CrtDbgBreak();
#else
	UNUSED_VARIABLE(pszExpression);
	UNUSED_VARIABLE(pszFile);
	UNUSED_VARIABLE(nLine);
#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Function to write a message to the debugger output. This is a printf() style
//! function for outputing debugging messages. It uses vsprintf() and so is
//! restricted to the types it can handle.

void TraceEx(const tchar* pszFormat, ...)
{
	va_list	args;

	va_start(args, pszFormat);

#ifdef CORE_CRTDBG_ENABLED
	// Output using CRT function.
	if (_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "%s", T2A(FmtEx(pszFormat, args).c_str())) == 1)
		_CrtDbgBreak();
#endif

	va_end(args);
}

#endif // _DEBUG

////////////////////////////////////////////////////////////////////////////////
//! Write a message to the debugger stream in both Debug and Release builds.
//! Unlike TraceEx() this goes directly to OutputDebugString().

void DebugWrite(const tchar* pszFormat, ...)
{
	va_list	args;

	va_start(args, pszFormat);

	::OutputDebugString(FmtEx(pszFormat, args).c_str());

	va_end(args);
}

//namespace Core
}
