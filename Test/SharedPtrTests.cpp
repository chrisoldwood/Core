////////////////////////////////////////////////////////////////////////////////
//! \file   SharedPtrTests.cpp
//! \brief  The unit tests for the SharedPtr class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <Core/SharedPtr.hpp>
#include "PtrTest.hpp"

namespace
{

class TestType
{
public:
	TestType()
		: m_value()
	{ }

	TestType(int value)
		: m_value(value)
	{ }

	int m_value;
};

}

TEST_SET(SharedPtr)
{
	typedef Core::SharedPtr<PtrTest> TestPtr;
	typedef Core::SharedPtr<Derived> DerivedPtr;
	typedef Core::SharedPtr<Unrelated> UnrelatedPtr;
	typedef Core::SharedPtr<TestType> MutableTestTypePtr;
	typedef Core::SharedPtr<const TestType> ImmutableTestTypePtr;

TEST_CASE("compilation should succeed")
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

	MutableTestTypePtr   mutableValue(new TestType);
	ImmutableTestTypePtr immutableValue;

//	mutableValue = immutableValue;			// No conversion allowed.
	immutableValue = mutableValue;			// [No] conversion allowed?

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

TEST_CASE("pointer can only be statically or dynamically cast to related types")
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
