////////////////////////////////////////////////////////////////////////////////
//! \file   RefCntPtrTests.cpp
//! \brief  The unit tests for the RefCntPtr class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/RefCntPtr.hpp>

class RefCntTest : public Core::RefCounted
{
public:
	virtual bool run()
	{
		return true;
	}
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

TEST_CASE("compilation should succeed")
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
	RefCntTest* expected = new RefCntTest;
	TestPtr     test(expected);

	TEST_TRUE(test.get() == expected);
}
TEST_CASE_END

TEST_CASE("owned pointer can be accessed as a pointer or reference")
{
	RefCntTest* expected = new RefCntTest;
	TestPtr     test(expected);

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
	TestPtr test(new RefCntTest);

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
	TestPtr test(new RefCntTest);

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
	TestPtr hasRealPtr(new RefCntTest);

	TEST_TRUE(!hasNullPtr);
	TEST_FALSE(!hasRealPtr);
}
TEST_CASE_END

TEST_CASE("[in]equivalence operator compares two pointers")
{
	TestPtr hasNullPtr;
	TestPtr hasRealPtr(new RefCntTest);

	TEST_TRUE(hasNullPtr == hasNullPtr);
	TEST_TRUE(hasNullPtr != hasRealPtr);
	TEST_TRUE(hasRealPtr == hasRealPtr);
}
TEST_CASE_END

TEST_CASE("reset empties the pointer when argument is null pointer")
{
	TestPtr test(new RefCntTest);

	test.reset();

	TEST_TRUE(test.get() == nullptr);
}
TEST_CASE_END

TEST_CASE("reset changes ownership when argument is not null pointer")
{
	TestPtr test(new RefCntTest);

	RefCntTest* expected = new RefCntTest;

	test.reset(expected);

	TEST_TRUE(test.get() == expected);
}
TEST_CASE_END

TEST_CASE("pointer can only be statically or dynamically cast to related types")
{
	TestPtr base1(new RefCntTest);
	TestPtr base2(new RefCntTest);
	TestPtr base3(new RefCntDerived);

	DerivedPtr derived1(new RefCntDerived);
	DerivedPtr derived2(new RefCntDerived);
	TestPtr    base4(derived1);

	UnrelatedPtr unrelated;

	base2 = derived1;
	base2 = derived1;

	TEST_TRUE(base2.get() == derived1.get());

	derived2 = Core::static_ptr_cast<RefCntDerived>(base3);

	TEST_TRUE(derived2.get() == base3.get());

	derived2 = Core::dynamic_ptr_cast<RefCntDerived>(base3);

	TEST_TRUE(derived2.get() == base3.get());

	unrelated = Core::dynamic_ptr_cast<RefCntUnrelated>(base1);

	TEST_TRUE(unrelated.get() == nullptr);
}
TEST_CASE_END

TEST_CASE("smart pointer unaware functions can attach a pointer to an empty instance")
{
	TestPtr test;

	RefCntTest* expected = new RefCntTest;

	*attachTo(test) = expected;

	TEST_TRUE(test.get() == expected);
}
TEST_CASE_END

TEST_CASE("attaching a pointer to a non-empty smart pointer throws an exception")
{
	TestPtr test(new RefCntTest);

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
	const TestPtr test(new RefCntTest);

	TEST_FALSE(test.empty());
}
TEST_CASE_END

}
TEST_SET_END
