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

////////////////////////////////////////////////////////////////////////////////
//! The entry point for the test harness.

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	TEST_SUITE_BEGIN
	{
		TestDebug();
		TestMisc();
		TestSharedPtr();
		TestUniquePtr();
		TestArrayPtr();
		TestRefCounted();
		TestStrings();
		TestCmdLineParser();

		Core::SetTestRunFinalStatus(true);
	}
	TEST_SUITE_END
}
