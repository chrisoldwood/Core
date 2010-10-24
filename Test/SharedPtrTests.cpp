////////////////////////////////////////////////////////////////////////////////
//! \file   TestSharedPtr.cpp
//! \brief  The unit tests for the SharedPtr class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/SharedPtr.hpp>
#include "PtrTest.hpp"

TEST_SET(SharedPtr)
{
	typedef Core::SharedPtr<PtrTest> TestPtr;
	typedef Core::SharedPtr<Derived> DerivedPtr;
	typedef Core::SharedPtr<Unrelated> UnrelatedPtr;

TEST_CASE(SharedPtr, compilationFails)
{
	TestPtr test1;
//	TestPtr test2 = new PtrTest;			// No implicit construction.
	TestPtr test3 = TestPtr(new PtrTest);
	TestPtr test4(new PtrTest);
	TestPtr test5(test3);

//	PtrTest* ptr = test1;					// No implicit conversion.

//	TEST_FALSE((test1 == NULL) || (test1 != NULL));	// No implicit construction.

//	delete test1;							// No implicit conversion.

	TestPtr      base(new PtrTest);
	UnrelatedPtr unrelated;

//	unrelated = Core::static_ptr_cast<Unrelated>(base);	// No explicit conversion.

	TEST_PASSED("compilation succeeded");
}
TEST_CASE_END

TEST_CASE(SharedPtr, accessors)
{
	TestPtr test1;
	TestPtr test2 = TestPtr(new PtrTest);

	TEST_TRUE(test1.get() == nullptr);
	TEST_TRUE(test2.get() != nullptr);

	test1 = test2;
	test1 = test1;

	TEST_TRUE(test1.get() == test2.get());

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

TEST_CASE(SharedPtr, comparison)
{
	TestPtr test1;
	TestPtr test2;
	TestPtr test3 = TestPtr(new PtrTest);

	TEST_TRUE(!test1);
	TEST_FALSE(!test3);

	TEST_TRUE(test1 == test2);
	TEST_TRUE(test1 != test3);

	test1.reset(new PtrTest);

	TEST_TRUE(test1.get() != nullptr);

	test1.reset();

	TEST_TRUE(test1.get() == nullptr);

	TEST_THROWS(*test1);
	TEST_THROWS(test1->release());
}
TEST_CASE_END

TEST_CASE(SharedPtr, freeFunctions)
{
	TestPtr base1(new PtrTest);
	TestPtr base2(new PtrTest);
	TestPtr base3(new Derived);

	DerivedPtr derived1(new Derived);
	DerivedPtr derived2(new Derived);
	TestPtr    base4(derived1);

	UnrelatedPtr unrelated;

	base2 = derived1;
	base2 = derived1;

	TEST_TRUE(base2.get() == derived1.get());

	derived2 = Core::static_ptr_cast<Derived>(base3);

	TEST_TRUE(derived2.get() == base3.get());

	derived2 = Core::dynamic_ptr_cast<Derived>(base3);

	TEST_TRUE(derived2.get() == base3.get());

	unrelated = Core::dynamic_ptr_cast<Unrelated>(base1);

	TEST_TRUE(unrelated.get() == nullptr);
}
TEST_CASE_END

}
TEST_SET_END
