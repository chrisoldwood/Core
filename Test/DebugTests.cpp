////////////////////////////////////////////////////////////////////////////////
//! \file   TestDebug.cpp
//! \brief  The unit tests for the debugging functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>

#ifdef __GNUG__
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

TEST_SET(Debug)
{

TEST_CASE(Debug, compilation)
{
	bool isTrue  = true;
	bool isFalse = false;

	ASSERT(isTrue);
	DEBUG_USE_ONLY(isTrue);

#ifdef CORE_CRTDBG_ENABLED
	int oldMode = _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
	ASSERT(isFalse);
	_CrtSetReportMode(_CRT_ASSERT, oldMode);
#else
	DEBUG_USE_ONLY(isFalse);
#endif

	STATIC_ASSERT(true);
//	STATIC_ASSERT(false);	// Not compilable.

	TEST_PASSED("compilation succeeded");
}
TEST_CASE_END

TEST_CASE(Debug, traceOutput)
{
	TRACE1(TXT("Test TraceEx(\"%s\")\n"), TXT("Hello World"));

	Core::debugWrite(TXT("Test DebugWrite(\"%s\")\n"), TXT("Hello World"));

	TEST_PASSED("function invoked");
}
TEST_CASE_END
/*
TEST_CASE(Debug, leakReporting)
{
//	strcpy(static_cast<char*>(malloc(10)), "Malloc");
//	strcpy(new char[5], "new");
}
TEST_CASE_END
*/
}
TEST_SET_END
