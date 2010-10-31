////////////////////////////////////////////////////////////////////////////////
//! \file   ArrayPtrTests.cpp
//! \brief  The unit tests for the ArrayPtr class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/ArrayPtr.hpp>

TEST_SET(ArrayPtr)
{
	typedef Core::ArrayPtr<int> TestPtr;

TEST_CASE("compilation only succeeds for explicit conversions")
{
	TestPtr test1;
//	TestPtr test2 = new int[1];				// No implicit construction.
//	TestPtr test3 = TestPtr(new int[1]);	// Not copy constructable.
	TestPtr test4(new int[1]);
//	TestPtr test5(test4);					// Not copy constructable.

//	test1 = test4;							// Not assignable.
//	int pRaw1[1] = test1;					// No implicit conversion.

//	TEST_FALSE((test1 == NULL) || (test1 != NULL));		// No implicit construction.

//	delete[] test1;							// No implicit conversion.

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
	int*    expected = new int[1];
	TestPtr test(expected);

	TEST_TRUE(test.get() == expected);
}
TEST_CASE_END

TEST_CASE("owned pointer can be accessed as a pointer or reference")
{
	int*    expected = new int[1];
	TestPtr test(expected);

	TEST_TRUE(test.get() == &test.getRef());
}
TEST_CASE_END

TEST_CASE("retrieving reference when pointer is null throws an exception")
{
	TestPtr test;

	TEST_THROWS(test.getRef());
}
TEST_CASE_END

TEST_CASE("not operator returns if the contained pointer is null or not")
{
	TestPtr hasNullPtr;
	TestPtr hasRealPtr(new int[1]);

	TEST_TRUE(!hasNullPtr);
	TEST_FALSE(!hasRealPtr);
}
TEST_CASE_END

TEST_CASE("[in]equivalence operator compares two pointers")
{
	TestPtr hasNullPtr;
	TestPtr hasRealPtr(new int[1]);

	TEST_TRUE(hasNullPtr == hasNullPtr);
	TEST_TRUE(hasNullPtr != hasRealPtr);
	TEST_TRUE(hasRealPtr == hasRealPtr);
}
TEST_CASE_END

TEST_CASE("reset empties the pointer when argument is null pointer")
{
	TestPtr test(new int[1]);

	test.reset();

	TEST_TRUE(test.get() == nullptr);
}
TEST_CASE_END

TEST_CASE("reset changes ownership when argument is not null pointer")
{
	TestPtr test(new int[1]);

	int* expected = new int[2];

	test.reset(expected);

	TEST_TRUE(test.get() == expected);
}
TEST_CASE_END

TEST_CASE("detach releases ownership of the pointer")
{
	int array[1] = { 12345678 };

	TestPtr test(array);

	TEST_TRUE(test.get() == array);

	test.detach();

	TEST_TRUE(test.get() == nullptr);
}
TEST_CASE_END

TEST_CASE("index operator returns a const reference to an element when pointer is const")
{
	int* array = new int[1];

	array[0] = 12345678;

	const TestPtr test(array);

	TEST_TRUE(test[0] == array[0]);
}
TEST_CASE_END

TEST_CASE("index operator returns a non-const reference to an element when pointer is not const")
{
	int array[1] = { 12345678 };

	TestPtr test(array);

	TEST_TRUE(test[0] == array[0]);

	test[0] = 87654321;

	TEST_TRUE(test[0] == array[0]);

	test.detach();
}
TEST_CASE_END

TEST_CASE("indexing a null pointer throws an exception")
{
	TestPtr test1;

	TEST_THROWS(test1[0]);

	const TestPtr test2;

	TEST_THROWS(test2[0]);
}
TEST_CASE_END

TEST_CASE("smart pointer unaware functions can attach a pointer to an empty instance")
{
	TestPtr test;

	int* array = new int[1];

	*attachTo(test) = array;

	TEST_TRUE(test.get() == array);
}
TEST_CASE_END

}
TEST_SET_END
