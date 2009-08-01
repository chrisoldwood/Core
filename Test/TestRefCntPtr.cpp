////////////////////////////////////////////////////////////////////////////////
//! \file   TestRefCntPtr.cpp
//! \brief  The unit tests for the RefCntPtr class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/RefCntPtr.hpp>

class RefCntTest : public Core::RefCounted
{
public:
	virtual void run()
	{ }
};

class RefCntDerived : public RefCntTest
{
};

class RefCntUnrelated : public Core::RefCounted
{
};

TEST_SET(RefCntPtr)
{
	typedef Core::RefCntPtr<RefCntTest> TestPtr;
	typedef Core::RefCntPtr<RefCntDerived> DerivedPtr;
	typedef Core::RefCntPtr<RefCntUnrelated> UnrelatedPtr;

TEST_CASE(RefCntPtr, compilationFails)
{
	TestPtr test1;
//	TestPtr test2 = new RefCntTest;				// No implicit construction.
	TestPtr test3 = TestPtr(new RefCntTest);
	TestPtr test4(new RefCntTest);
	TestPtr test5(test3);

//	RefCntTest* ptr = test1;					// No implicit conversion.

//	TEST_FALSE((test1 == NULL) || (test1 != NULL));	// No implicit construction.

//	delete test1;								// No implicit conversion.

	TestPtr      base(new RefCntTest);
	UnrelatedPtr unrelated;

//	unrelated = Core::static_ptr_cast<RefCntUnrelated>(base);	// No explicit conversion.
}
TEST_CASE_END

TEST_CASE(RefCntPtr, accessors)
{
	TestPtr test1;
	TestPtr test2 = TestPtr(new RefCntTest);

	TEST_TRUE(test1.get() == nullptr);
	TEST_TRUE(test2.get() != nullptr);

	test2->run();
	(*test2).run();

	RefCntTest* ptr = test2.get();
	RefCntTest& ref = test2.getRef();

	TEST_TRUE(ptr  == test2.get());
	TEST_TRUE(&ref == test2.get());

	ptr->run();
	ref.run();
}
TEST_CASE_END

TEST_CASE(RefCntPtr, mutators)
{
	TestPtr test1;
	TestPtr test2 = TestPtr(new RefCntTest);

	test1 = test2;
	test1 = test1;

	TEST_TRUE(test1.get() == test2.get());

	test1.reset(new RefCntTest);

	TEST_TRUE(test1.get() != nullptr);

	test1.reset();

	TEST_TRUE(test1.get() == nullptr);
	TEST_THROWS(*test1);
	TEST_THROWS(test1->decRefCount());
}
TEST_CASE_END

TEST_CASE(ArrayPtr, comparison)
{
	TestPtr test1;
	TestPtr test2;
	TestPtr test3 = TestPtr(new RefCntTest);

	TEST_TRUE(!test1);
	TEST_FALSE(!test3);

	TEST_TRUE(test1 == test2);
	TEST_TRUE(test1 != test3);
}
TEST_CASE_END

TEST_CASE(RefCntPtr, freeFunctions)
{
	TestPtr base1(new RefCntTest);
	TestPtr base2(new RefCntTest);
	TestPtr base3(new RefCntDerived);

	DerivedPtr derived1(new RefCntDerived);
	DerivedPtr derived2(new RefCntDerived);
	TestPtr    base4(derived1);

	UnrelatedPtr pUnrelated;

	base2 = derived1;
	base2 = derived1;

	TEST_TRUE(base2.get() == derived1.get());

	derived2 = Core::static_ptr_cast<RefCntDerived>(base3);

	TEST_TRUE(derived2.get() == base3.get());

	derived2 = Core::dynamic_ptr_cast<RefCntDerived>(base3);

	TEST_TRUE(derived2.get() == base3.get());

	pUnrelated = Core::dynamic_ptr_cast<RefCntUnrelated>(base1);

	TEST_TRUE(pUnrelated.get() == nullptr);

	TEST_TRUE(*attachTo(pUnrelated) == nullptr);
}
TEST_CASE_END

}
TEST_SET_END
