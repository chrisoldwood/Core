////////////////////////////////////////////////////////////////////////////////
//! \file   TestMisc.cpp
//! \brief  The unit tests for misc types and functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/NotImplException.hpp>
#include <Core/NullPtrException.hpp>
#include <Core/InvalidArgException.hpp>
#include <Core/BadLogicException.hpp>
#include <Core/ParseException.hpp>
#include <Core/ConfigurationException.hpp>
#include <Core/RuntimeException.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The test concrete class.

class CopyTest : public Core::NotCopyable
{
};

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the NotCopyable class.

void testNotCopyable()
{
	CopyTest oTest1;
//	CopyTest oTest2(oTest1);	// Shouldn't compile.
	CopyTest oTest3;
//	oTest3 = oTest1;			// Shouldn't compile.
}

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the Exception classes.

void testException()
{
//	Core::Exception dc;											// Shouldn't compile.
//	Core::Exception cc(Core::NullPtrException(TXT("Test")));	// Shouldn't compile.
	Core::NotImplException e(TXT("Test"));
	Core::NullPtrException npe(TXT("Test"));
	Core::InvalidArgException iae(TXT("Test"));
	Core::BadLogicException ble(TXT("Test"));
	Core::ParseException pe(TXT("Test"));
	Core::ConfigurationException ce(TXT("Test"));
	Core::RuntimeException re(TXT("Test"));

	TEST_TRUE(tstrcmp(e.twhat(), TXT("Test")) == 0);
	
	Core::debugWrite(TXT("%s\n"), e.twhat());
//	Core::debugWrite("%s", e.what());		// Shouldn't compile.
}

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for other misc types and functions.

void testMisc()
{
	testNotCopyable();
	testException();
}