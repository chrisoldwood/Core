////////////////////////////////////////////////////////////////////////////////
//! \file   TestDebug.cpp
//! \brief  The unit tests for the debugging functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>

#ifdef __GNUG__
#pragma GCC diagnostic ignored "-Wunused-value"
#endif

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the debugging functions.

void testDebug()
{
	bool bTrue  = true;
//	bool bFalse = false;

	ASSERT(bTrue);
	DEBUG_USE_ONLY(bTrue);

//	ASSERT(bFalse);

	STATIC_ASSERT(true);

//	STATIC_ASSERT(false);

	TRACE1(TXT("Test TraceEx(\"%s\")\n"), TXT("Hello World"));

	Core::debugWrite(TXT("Test DebugWrite(\"%s\")\n"), TXT("Hello World"));

//	strcpy(static_cast<char*>(malloc(10)), "Malloc");
//	strcpy(new char[5], "new");
}
