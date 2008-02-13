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
extern void TestPointers();
extern void TestStrings();

////////////////////////////////////////////////////////////////////////////////
//! The entry point for the test harness.

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
#ifdef _DEBUG
	Core::EnableLeakReporting(true);
#endif

	try
	{
		TestDebug();
		TestMisc();
		TestPointers();
		TestStrings();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	Core::WriteTestsSummary();

	return Core::GetTestProcessResult();
}
