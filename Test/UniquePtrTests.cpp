////////////////////////////////////////////////////////////////////////////////
//! \file   UniquePtrTests.cpp
//! \brief  The unit tests for the UniquePtr class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/UniquePtr.hpp>
#include "PtrTest.hpp"

TEST_SET(UniquePtr)
{
	typedef Core::UniquePtr<PtrTest> TestPtr;

TEST_CASE("compilation should succeed")
{
	TestPtr test1;
//	TestPtr test2 = new PtrTest;			// No implicit construction.
//	TestPtr test3 = TestPtr(new PtrTest);	// Not copy constructable.
	TestPtr test4(new PtrTest);
//	TestPtr test5(test4);					// Not copy constructable.
	TestPtr test6;

//	test1 = test4;							// Not assignable.
//	PtrTest* ptr = test1;					// No implicit conversion.

//	TEST_FALSE((test1 == NULL) || (test1 != NULL));		// No implicit construction.

//	delete test1;							// No implicit conversion.

	TEST_PASSED("compilation succeeded");
}
TEST_CASE_END

TEST_CASE("initial state is a null pointer")
{
	TestPtr test;

	TEST_TRUE(test.get() == nullptr);
}
TEST_CASE_END

TEST_CASE("construction with a pointer passes ownership")
{
	PtrTest* expected = new PtrTest;
	TestPtr  test(expected);

	TEST_TRUE(test.get() == expected);
}
TEST_CASE_END

TEST_CASE("owned pointer can be accessed as a pointer or reference")
{
	PtrTest* expected = new PtrTest;
	TestPtr  test(expected);

	TEST_TRUE(test.get() == &test.getRef());
}
TEST_CASE_END

TEST_CASE("retrieving reference when pointer is null throws an exception")
{
	TestPtr test;

	TEST_THROWS(test.getRef());
}
TEST_CASE_END

TEST_CASE("operator -> returns owned pointer")
{
	TestPtr test(new PtrTest);

	TEST_TRUE(test->run());
}
TEST_CASE_END

TEST_CASE("operator -> throws when the owned pointer is null")
{
	TestPtr test;

	TEST_THROWS(test->run());
}
TEST_CASE_END

TEST_CASE("operator * returns owned pointer as a reference")
{
	TestPtr test(new PtrTest);

	TEST_TRUE((*test).run());
}
TEST_CASE_END

TEST_CASE("operator * throws when the owned pointer is null")
{
	TestPtr test;

	TEST_THROWS((*test).run());
}
TEST_CASE_END

TEST_CASE("not operator returns if the contained pointer is null or not")
{
	TestPtr hasNullPtr;
	TestPtr hasRealPtr(new PtrTest);

	TEST_TRUE(!hasNullPtr);
	TEST_FALSE(!hasRealPtr);
}
TEST_CASE_END

TEST_CASE("[in]equivalence operator compares two pointers")
{
	TestPtr hasNullPtr;
	TestPtr hasRealPtr(new PtrTest);

	TEST_TRUE(hasNullPtr == hasNullPtr);
	TEST_TRUE(hasNullPtr != hasRealPtr);
	TEST_TRUE(hasRealPtr == hasRealPtr);
}
TEST_CASE_END

TEST_CASE("reset empties the pointer when argument is null pointer")
{
	TestPtr test(new PtrTest);

	test.reset();

	TEST_TRUE(test.get() == nullptr);
}
TEST_CASE_END

TEST_CASE("reset changes ownership when argument is not null pointer")
{
	TestPtr test(new PtrTest);

	PtrTest* expected = new PtrTest;

	test.reset(expected);

	TEST_TRUE(test.get() == expected);
}
TEST_CASE_END

TEST_CASE("detach releases ownership of the pointer")
{
	PtrTest  value;
	PtrTest* expected = &value;

	TestPtr test(expected);

	TEST_TRUE(test.get() == expected);

	test.detach();

	TEST_TRUE(test.get() == nullptr);
}
TEST_CASE_END

TEST_CASE("smart pointer unaware functions can attach a pointer to an empty instance")
{
	TestPtr test;

	PtrTest* expected = new PtrTest;

	*attachTo(test) = expected;

	TEST_TRUE(test.get() == expected);
}
TEST_CASE_END

TEST_CASE("attaching a pointer to a non-empty smart pointer throws an exception")
{
	TestPtr test(new PtrTest);

	TEST_THROWS(attachTo(test));
}
TEST_CASE_END

TEST_CASE("the pointer is empty when it owns nothing")
{
	const TestPtr test;

	TEST_TRUE(test.empty());
}
TEST_CASE_END

TEST_CASE("the pointer is not empty when it owns something")
{
	const TestPtr test(new PtrTest);

	TEST_FALSE(test.empty());
}
TEST_CASE_END

}
TEST_SET_END
