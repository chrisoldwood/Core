////////////////////////////////////////////////////////////////////////////////
//! \file   UnitTest.cpp
//! \brief  Functions for writing simple unit tests.
//! \author Chris Oldwood

#include "Common.hpp"
#include "StringUtils.hpp"
#include <iostream>

// Avoid bringing in <windows.h>.
extern "C" void __stdcall DebugBreak();
extern "C" int __stdcall IsDebuggerPresent();

namespace Core
{

//! The number of tests that passed.
static size_t s_nPassed = 0;
//! The number of tests that failed
static size_t s_nFailed = 0;

////////////////////////////////////////////////////////////////////////////////
//! Write the test results to stdout.

void WriteTestResult(const char* pszFile, size_t nLine, const tchar* pszExpression, bool bPassed)
{
	// The result strings.
	const tchar* pszResult = (bPassed) ? TXT("PASSED") : TXT("FAILED");

	// Strip path from filename, if present.
	const char* pszFileName = strrchr(pszFile, '\\');

	if (pszFileName != nullptr)
		++pszFileName;
	else
		pszFileName = pszFile;

	// Write the result to the debugger output.
	std::tcout << Core::Fmt(TXT("%s [%hs, %4u] %s"), pszResult, pszFileName, nLine, pszExpression) << std::endl;

	// Break into debugger, if present.
	if (!bPassed && ::IsDebuggerPresent())
		::DebugBreak();

	// Update stats.
	if (bPassed)
		++s_nPassed;
	else
		++s_nFailed;
}

////////////////////////////////////////////////////////////////////////////////
// Write the summary of the test results to stdout.

void WriteTestsSummary()
{
	std::tcout << std::endl << Core::Fmt(TXT("Test Results: %u Passed %u Failed"), s_nPassed, s_nFailed) << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// Get the test process result code.

int GetTestProcessResult()
{
	return (s_nFailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

//namespace Core
}
