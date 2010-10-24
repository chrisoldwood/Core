////////////////////////////////////////////////////////////////////////////////
//! \file   InterlockedTests.cpp
//! \brief  The unit tests for the Interlocked functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/Interlocked.hpp>

TEST_SET(Interlocked)
{

TEST_CASE("atomicIncrement")
{
	long value = 1;

	TEST_TRUE(Core::atomicIncrement(value) == 2);
}
TEST_CASE_END

TEST_CASE("atomicDecrement")
{
	long value = 2;

	TEST_TRUE(Core::atomicDecrement(value) == 1);
}
TEST_CASE_END

}
TEST_SET_END
