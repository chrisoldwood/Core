////////////////////////////////////////////////////////////////////////////////
//! \file   TestUniquePtr.cpp
//! \brief  The unit tests for the UniquePtr class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/UniquePtr.hpp>
#include "PtrTest.hpp"

TEST_SET(UniquePtr)
{
	typedef Core::UniquePtr<PtrTest> TestPtr;

TEST_CASE(UniquePtr, compilationFails)
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

TEST_CASE(UniquePtr, accessors)
{
	TestPtr test1;
	TestPtr test2(new PtrTest);

	TEST_TRUE(test1.get() == nullptr);
	TEST_TRUE(test2.get() != nullptr);

	test2->run();
	(*test2).run();

	PtrTest* ptr = test2.get();
	PtrTest& ref = test2.getRef();

	TEST_TRUE(ptr  == test2.get());
	TEST_TRUE(&ref == test2.get());

	ptr->run();
	ref.run();
}
TEST_CASE_END

TEST_CASE(UniquePtr, comparison)
{
	TestPtr test1;
	TestPtr test2;
	TestPtr test3(new PtrTest);

	TEST_TRUE(!test1);
	TEST_FALSE(!test3);

	TEST_TRUE(test1 == test2);
	TEST_TRUE(test1 != test3);
}
TEST_CASE_END

TEST_CASE(UniquePtr, mutators)
{
	TestPtr test;

	test.reset(new PtrTest);

	TEST_TRUE(test.get() != nullptr);

	test.reset();

	TEST_TRUE(test.get() == nullptr);

	TEST_THROWS(*test);
	TEST_THROWS(test->release());

	delete test.detach();

	TEST_TRUE(test.get() == nullptr);
}
TEST_CASE_END

TEST_CASE(UniquePtr, freeFunctions)
{
	PtrTest* ptr = new PtrTest;

	TestPtr test;

	*attachTo(test) = ptr;

	TEST_TRUE(test.get() == ptr);
}
TEST_CASE_END

}
TEST_SET_END
