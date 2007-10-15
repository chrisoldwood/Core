////////////////////////////////////////////////////////////////////////////////
//! \file   Debug.cpp
//! \brief  Debug reporting functions.
//! \author Chris Oldwood

#include "Common.hpp"
#include <stdio.h>
#include <stdarg.h>
#include "StringUtils.hpp"

////////////////////////////////////////////////////////////////////////////////
// Avoid bringing in <windows.h>.

extern "C" void __stdcall OutputDebugStringA(const char*);
extern "C" void __stdcall OutputDebugStringW(const wchar_t*);

#ifdef _UNICODE
#define	OutputDebugString	OutputDebugStringW
#else
#define	OutputDebugString	OutputDebugStringA
#endif

namespace Core
{

//! Size of debug output buffer in characters.
const size_t MAX_CHARS = 1024;

////////////////////////////////////////////////////////////////////////////////
// Debug build only functions.

#ifdef _DEBUG

////////////////////////////////////////////////////////////////////////////////
//! The function invoked when an ASSERT fails. It displays a message box with
//! details about why and where the assert failed. If asked it will also cause
//! an INT 3 to kick in the debugger.

void AssertFail(const char* pszExpression, const char* pszFile, uint iLine)
{
	// Output using CRT function.
	if (_CrtDbgReport(_CRT_ASSERT, pszFile, iLine, NULL, pszExpression) == 1)
		_CrtDbgBreak();
}

////////////////////////////////////////////////////////////////////////////////
//! Function to write a message to the debugger output. This is a printf() style
//! function for outputing debugging messages. It uses vsprintf() and so is
//! restricted to the types it can handle.

void TraceEx(const char* pszFormat, ...)
{
	va_list	args;

	va_start(args, pszFormat);

	// Output using CRT function.
	if (_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, FmtEx(pszFormat, args).c_str()) == 1)
		_CrtDbgBreak();

	va_end(args);
}

#endif // _DEBUG

////////////////////////////////////////////////////////////////////////////////
//! Write a message to the debugger stream in both Debug and Release builds.
//! Unlike TraceEx() this goes directly to OutputDebugString().

void DebugWrite(const char* pszFormat, ...)
{
	va_list	args;

	va_start(args, pszFormat);

	::OutputDebugString(FmtEx(pszFormat, args).c_str());

	va_end(args);
}

//namespace Core
}
