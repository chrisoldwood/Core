////////////////////////////////////////////////////////////////////////////////
//! \file   Test.cpp
//! \brief  The test harness entry point.
//! \author Chris Oldwood

#include "stdafx.h"
#include <tchar.h>
#include <Core/UnitTest.hpp>

////////////////////////////////////////////////////////////////////////////////
// The test group functions.

extern void TestDebug();
extern void TestMisc();
extern void TestSharedPtr();
extern void TestUniquePtr();
extern void TestArrayPtr();
extern void TestRefCounted();
extern void TestRefCntPtr();
extern void TestStrings();
extern void TestCmdLineParser();
extern void TestAlgorithm();
extern void TestTokeniser();

////////////////////////////////////////////////////////////////////////////////
//! The entry point for the test harness.

int _tmain(int argc, _TCHAR* argv[])
{
	TEST_SUITE_BEGIN(argc, argv)
	{
		TEST_CASE(TestDebug);
		TEST_CASE(TestMisc);
		TEST_CASE(TestSharedPtr);
		TEST_CASE(TestUniquePtr);
		TEST_CASE(TestArrayPtr);
		TEST_CASE(TestRefCounted);
		TEST_CASE(TestStrings);
		TEST_CASE(TestCmdLineParser);
		TEST_CASE(TestAlgorithm);
		TEST_CASE(TestTokeniser);

		Core::SetTestRunFinalStatus(true);
	}
	TEST_SUITE_END
}
