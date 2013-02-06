////////////////////////////////////////////////////////////////////////////////
//! \file   RefCountedTests.cpp
//! \brief  The unit tests for the RefCounted class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/RefCounted.hpp>

TEST_SET(RefCounted)
{

TEST_CASE("compilation should succeed")
{
//	Core::RefCounted test1;					// Not default constructable.

	Core::RefCounted* test1 = nullptr;
//	Core::RefCounted  test2(*test1);		// Not copy constructable.

	Core::RefCounted* test2 = nullptr;

//	*test1 = *test2;						// Not assignable.

#ifdef __GNUG__
	test1 = test2;
	test2 = test1;
#endif
	test1 = nullptr;
	test2 = nullptr;

	TEST_PASSED("compilation succeeded");
}
TEST_CASE_END

	static bool dtorInvoked;

	class Test : public Core::RefCounted
	{
	public:
		Test()
		{
			dtorInvoked = false;
		}
	private:
		virtual ~Test()
		{
			dtorInvoked = true;
		}
	};

TEST_CASE("object starts with a reference count of 1")
{
	Core::RefCounted* test = new Test;

	TEST_TRUE(test->refCount() == 1);

	test->decRefCount();
}
TEST_CASE_END

TEST_CASE("the reference count can be explicitly incremented and decremented")
{
	Core::RefCounted* test = new Test;

	long count = test->refCount();

	TEST_TRUE(test->incRefCount() != count);
	TEST_TRUE(test->decRefCount() == count);

	test->decRefCount();
}
TEST_CASE_END

TEST_CASE("object is destroyed when there are no remaining refernces")
{
	Core::RefCounted* test = new Test;

	TEST_FALSE(dtorInvoked);

	test->decRefCount();

	TEST_TRUE(dtorInvoked);
}
TEST_CASE_END

}
TEST_SET_END
