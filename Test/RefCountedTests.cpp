////////////////////////////////////////////////////////////////////////////////
//! \file   RefCountedTests.cpp
//! \brief  The unit tests for the RefCounted class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/RefCounted.hpp>

TEST_SET(RefCounted)
{

TEST_CASE("compilationFails")
{
//	Core::RefCounted test1;					// Not default constructable.

	Core::RefCounted* test1 = nullptr;
//	Core::RefCounted  test2(*test1);		// Not copy constructable.

	Core::RefCounted* test2 = nullptr;

//	*test1 = *test2;						// Not assignable.

	test1 = nullptr;
	test2 = nullptr;


	TEST_PASSED("compilation succeeded");
}
TEST_CASE_END

TEST_CASE("lifetimeManaged")
{
	static bool dtorInvoked = false;

	class RefCountedTest : public Core::RefCounted
	{
		~RefCountedTest()
		{
			dtorInvoked = true;
		}
	};

	Core::RefCounted* test = nullptr;

	test = new RefCountedTest;

	TEST_TRUE(test->refCount() == 1);
	TEST_TRUE(test->incRefCount() == 2);
	TEST_TRUE(test->decRefCount() == 1);

	test->decRefCount();

	TEST_TRUE(dtorInvoked);
}
TEST_CASE_END

}
TEST_SET_END
