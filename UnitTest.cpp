////////////////////////////////////////////////////////////////////////////////
//! \file   UnitTest.cpp
//! \brief  Functions for writing simple unit tests.
//! \author Chris Oldwood

#include "Common.hpp"
#include "UnitTest.hpp"
#include "StringUtils.hpp"
#include "tiostream.hpp"
#include "CmdLineException.hpp"
#include <stdlib.h>

namespace Core
{

//! The overall state of the test run.
static bool s_bSuccess = false;
//! The number of tests that passed.
static uint s_nPassed = 0;
//! The number of tests that failed
static uint s_nFailed = 0;

////////////////////////////////////////////////////////////////////////////////
//! Parse the command line. This extracts the list of test cases to run.

void parseCmdLine(int argc, tchar* argv[], TestCases& cases)
{
	tchar** arg = argv;
	tchar** end = argv + argc;

	// First argument should be the program name.
	if (arg == end)
		throw CmdLineException(TXT("The first argument must be the process name"));

	++arg;

	// Build the test case list.
	while (arg != end)
	{
		cases.insert(Core::createLower(*arg));
		++arg;
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Write the test results to stdout.

void WriteTestResult(const char* pszFile, size_t nLine, const tchar* pszExpression, bool bPassed)
{
	// The result strings.
	const tchar* pszResult = (bPassed) ? TXT("Passed") : TXT("FAILED");

	// Strip path from filename, if present.
	const char* pszFileName = strrchr(pszFile, '\\');

	if (pszFileName != nullptr)
		++pszFileName;
	else
		pszFileName = pszFile;

	// Write the result to the debugger output.
	tcout << Core::Fmt(TXT("%s [%hs, %4u] %s"), pszResult, pszFileName, nLine, pszExpression) << std::endl;

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
//! Set how the test run completed.

void SetTestRunFinalStatus(bool bSuccess)
{
	s_bSuccess = bSuccess;
}

////////////////////////////////////////////////////////////////////////////////
//! Write the summary of the test results to the debugger stream and stdout.

void WriteTestsSummary()
{
	tstring str = Core::Fmt(TXT("Test Results: %u Passed %u Failed"), s_nPassed, s_nFailed);

	if (!s_bSuccess)
		str += TXT(" [RUN TERMINATED ABORMALLY]");

	DebugWrite(TXT("%s\n"), str.c_str());
	std::tcout << std::endl << str << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the test process result code.

int GetTestProcessResult()
{
	return (s_bSuccess && (s_nFailed == 0)) ? EXIT_SUCCESS : EXIT_FAILURE;
}

//namespace Core
}
