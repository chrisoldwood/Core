////////////////////////////////////////////////////////////////////////////////
//! \file   NotCopyableTests.cpp
//! \brief  The unit tests for the NotCopyable class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/NotCopyable.hpp>

TEST_SET(NotCopyable)
{

TEST_CASE("compilationFails")
{
	class CopyTest : public Core::NotCopyable
	{
	};

//	Core::NotCopyable test1;	// Not constructible.

	CopyTest test2;
//	CopyTest test3(test2);		// Not copy constructible.

	CopyTest test4;
//	test4 = test2;				// Not assignable.

	TEST_PASSED("compilation succeeded");
}
TEST_CASE_END

}
TEST_SET_END
