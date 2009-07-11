////////////////////////////////////////////////////////////////////////////////
//! \file   TestInterlocked.cpp
//! \brief  The unit tests for the Interlocked functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/Interlocked.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the Interlocked functions.

void testInterlocked()
{
	long value = 1;

	TEST_TRUE(Core::atomicIncrement(value) == 2);
	TEST_TRUE(Core::atomicDecrement(value) == 1);
}
