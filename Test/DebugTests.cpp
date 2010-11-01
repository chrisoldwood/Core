////////////////////////////////////////////////////////////////////////////////
//! \file   DebugTests.cpp
//! \brief  The unit tests for the debugging functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>

#ifdef __GNUG__
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

TEST_SET(Debug)
{

TEST_CASE("compilation should succeed")
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
	UNUSED_VARIABLE(isFalse);
#endif

	STATIC_ASSERT(true);
//	STATIC_ASSERT(false);	// Not compilable.

	TEST_PASSED("compilation succeeded");
}
TEST_CASE_END

TEST_CASE("text can be written to the debugger output stream")
{
	TRACE1(TXT("Test TraceEx(\"%s\")\n"), TXT("Hello World"));

	Core::debugWrite(TXT("Test DebugWrite(\"%s\")\n"), TXT("Hello World"));

	TEST_PASSED("function invoked");
}
TEST_CASE_END
/*
TEST_CASE("memory leaks are reported at shutdown")
{
	strcpy(static_cast<char*>(malloc(10)), "malloc");
	strcpy(static_cast<char*>(calloc(1, 10)), "calloc");
	strcpy(static_cast<char*>(realloc(nullptr, 10)), "realloc");

	strcpy(new char[5], "new");
}
TEST_CASE_END
*/
}
TEST_SET_END
