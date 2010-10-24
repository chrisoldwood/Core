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

TEST_CASE("compilationFails")
{
	TestPtr test1;
//	TestPtr test2 = new int[1];				// No implicit construction.
//	TestPtr test3 = TestPtr(new int[1]);	// Not copy constructable.
	TestPtr test4(new int[1]);
//	TestPtr test5(test4);					// Not copy constructable.
	TestPtr test6;

//	test1 = test4;							// Not assignable.
//	int pRaw1[1] = test1;					// No implicit conversion.

//	TEST_FALSE((test1 == NULL) || (test1 != NULL));		// No implicit construction.

//	delete[] test1;							// No implicit conversion.

	TEST_PASSED("compilation succeeded");
}
TEST_CASE_END

TEST_CASE("accessors")
{
	TestPtr test1;
	TestPtr test2(new int[1]);

	TEST_TRUE(test1.get() == nullptr);
	TEST_TRUE(test2.get() != nullptr);

	int* ptr = test2.get();
	int& ref = test2.getRef();

	TEST_TRUE(ptr  == test2.get());
	TEST_TRUE(&ref == test2.get());
}
TEST_CASE_END

TEST_CASE("comparison")
{
	TestPtr test1;
	TestPtr test2;
	TestPtr test3(new int[1]);

	TEST_TRUE(!test1);
	TEST_FALSE(!test3);

	TEST_TRUE(test1 == test2);
	TEST_TRUE(test1 != test3);
}
TEST_CASE_END

TEST_CASE("mutation")
{
	int array[1] = { 12345678 };

	TestPtr test;

	test.reset(array);

	TEST_TRUE(test.get() != nullptr);
	TEST_TRUE(test[0] == 12345678);

	test.detach();

	TEST_TRUE(test.get() == nullptr);

	TEST_THROWS(*test);
	TEST_THROWS(test[0]);
}
TEST_CASE_END

TEST_CASE("freeFunctions")
{
	int* array = new int[1];

	TestPtr test;

	*attachTo(test) = array;

	TEST_TRUE(test.get() == array);
}
TEST_CASE_END

}
TEST_SET_END
