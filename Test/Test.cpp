////////////////////////////////////////////////////////////////////////////////
//! \file   Test.cpp
//! \brief  The test harness entry point.
//! \author Chris Oldwood

#include "stdafx.h"
#include <tchar.h>
#include <Core/UnitTest.hpp>

////////////////////////////////////////////////////////////////////////////////
// The test group functions.

extern void testDebug();
extern void testMisc();
extern void testSharedPtr();
extern void testUniquePtr();
extern void testArrayPtr();
extern void testRefCounted();
extern void testRefCntPtr();
extern void testStrings();
extern void testCmdLineParser();
extern void testAlgorithm();
extern void testTokeniser();
extern void testInterlocked();

////////////////////////////////////////////////////////////////////////////////
//! The entry point for the test harness.

int _tmain(int argc, _TCHAR* argv[])
{
	TEST_SUITE_BEGIN(argc, argv)
	{
		TEST_CASE(testDebug);
		TEST_CASE(testMisc);
		TEST_CASE(testSharedPtr);
		TEST_CASE(testUniquePtr);
		TEST_CASE(testArrayPtr);
		TEST_CASE(testRefCounted);
		TEST_CASE(testStrings);
		TEST_CASE(testCmdLineParser);
		TEST_CASE(testAlgorithm);
		TEST_CASE(testTokeniser);
		TEST_CASE(testInterlocked);

		Core::setTestRunFinalStatus(true);
	}
	TEST_SUITE_END
}
