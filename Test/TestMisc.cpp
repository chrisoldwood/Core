////////////////////////////////////////////////////////////////////////////////
//! \file   TestMisc.cpp
//! \brief  The unit tests for misc types and functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/NotImplException.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The test concrete class.

class CopyTest : public Core::NotCopyable
{
};

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the NotCopyable class.

void TestNotCopyable()
{
	CopyTest oTest1;
//	CopyTest oTest2(oTest1);	// Shouldn't compile.
	CopyTest oTest3;
//	oTest3 = oTest1;			// Shouldn't compile.
}

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the Exception classes.

void TestException()
{
//	Core::Exception x;			// Shouldn't compile.
	Core::NotImplException e;

	Core::DebugWrite(TXT("%s"), e.What());
//	Core::DebugWrite("%s", e.what());		// Shouldn't compile.
}

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for other misc types and functions.

void TestMisc()
{
	TestNotCopyable();
	TestException();
}
