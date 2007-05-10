/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		ASSERT.CPP
** COMPONENT:	Windows C++ Library.
** DESCRIPTION:	Debugging output functions.
**
*******************************************************************************
*/

#include "wcl.hpp"
#include <stdio.h>
#include <stdarg.h>

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

#ifdef _DEBUG

/******************************************************************************
** Function:	AssertFail()
**
** Description:	This is called when an ASSERT fails. It displays a message box
**				with details about why and where the assert failed. If asked
**				it will also cause an INT 3 to kick in the debugger.
**
** Parameters:	pszExpression	A string representation of the ASSERT.
**				pszFile			The file in which it occurs.
**				iLine			The line where it occurs.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void AssertFail(const char* pszExpression, const char* pszFile, uint iLine)
{
	// Output using CRT function.
	if (_CrtDbgReport(_CRT_ASSERT, pszFile, iLine, NULL, pszExpression) == 1)
		_CrtDbgBreak();
}

/******************************************************************************
** Function:	TraceEx()
**
** Description:	This is a printf() style function for outputing debugging
**				messages. It uses vsprintf() and so is restricted to the types
**				it can handle.
**
** Parameters:	pszFormat	The format of the output string.
**				...			Variable number of arguments.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void TraceEx(const char* pszFormat, ...)
{
	va_list	args;
	va_start(args, pszFormat);

	CString str;

	str.FormatEx(pszFormat, args);

	// Output using CRT function.
	if (_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, str) == 1)
		_CrtDbgBreak();

	va_end(args);
}

#endif // _DEBUG
