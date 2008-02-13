////////////////////////////////////////////////////////////////////////////////
//! \file   TestDebug.cpp
//! \brief  The unit tests for the debugging functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the debugging functions.

void TestDebug()
{
	bool bTrue  = true;
//	bool bFalse = false;

	ASSERT(bTrue);
	DEBUG_USE_ONLY(bTrue);

//	ASSERT(bFalse);

	STATIC_ASSERT(true);

//	STATIC_ASSERT(false);

	TRACE1(TXT("Test TraceEx(\"%s\")\n"), TXT("Hello World"));

	Core::DebugWrite(TXT("Test DebugWrite(\"%s\")\n"), TXT("Hello World"));

//	strcpy(static_cast<char*>(malloc(10)), "Malloc");
//	strcpy(new char[5], "new");
}
