////////////////////////////////////////////////////////////////////////////////
//! \file   NotCopyableTests.cpp
//! \brief  The unit tests for the NotCopyable class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <Core/NotCopyable.hpp>

class CopyTest : public Core::NotCopyable
{
};

class CopyTest2
{
public:
	CopyTest2()
	{ }

	CORE_NOT_COPYABLE(CopyTest2);
};

static void TakesReference(const CopyTest2&)
{
}

TEST_SET(NotCopyable)
{

TEST_CASE("compilation should succeed")
{
//	Core::NotCopyable test1;	// Not constructible.

	CopyTest test2;
//	CopyTest test3(test2);		// Not copy constructible.

	CopyTest test4;
//	test4 = test2;				// Not assignable.

	TEST_PASSED("compilation succeeded");
}
TEST_CASE_END

TEST_CASE("compilation should succeed for rvalue reference")
{
	TakesReference(CopyTest2());

	TEST_PASSED("compilation succeeded");
}
TEST_CASE_END

}
TEST_SET_END
