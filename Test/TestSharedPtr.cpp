////////////////////////////////////////////////////////////////////////////////
//! \file   TestSharedPtr.cpp
//! \brief  The unit tests for the SharedPtr class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/SharedPtr.hpp>
#include "PtrTest.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the SharedPtr class.

void testSharedPtr()
{
	typedef Core::SharedPtr<PtrTest> TestPtr;
	typedef Core::SharedPtr<Derived> DerivedPtr;
	typedef Core::SharedPtr<Unrelated> UnrelatedPtr;

	TestPtr pTest1;
//	TestPtr pTest2 = new PtrTest;			// Shouldn't compile.
	TestPtr pTest3 = TestPtr(new PtrTest);
	TestPtr pTest4(new PtrTest);
	TestPtr pTest5(pTest3);

	TEST_TRUE(pTest1.get() == nullptr);
	TEST_TRUE(pTest3.get() != nullptr);
	TEST_TRUE(pTest4.get() != nullptr);
	TEST_TRUE(pTest5.get() != nullptr);

	pTest1 = pTest3;
	pTest1 = pTest1;
//	PtrTest* pRaw1 = pTest1;				// Shouldn't compile.

	TEST_TRUE(pTest1.get() == pTest3.get());

	pTest1->run();
	(*pTest1).run();

	PtrTest* pRaw2 = pTest1.get();
	PtrTest& oRef  = pTest1.getRef();

	TEST_TRUE(pRaw2 == pTest1.get());
	TEST_TRUE(&oRef == pTest1.get());

	pRaw2->run();
	oRef.run();

//	TEST_FALSE((pTest1 == NULL) || (pTest1 != NULL));	// Shouldn't compile.
	TEST_FALSE(!pTest1);

	TEST_TRUE(pTest1 == pTest3);
	TEST_TRUE(pTest1 != pTest4);

	pTest1.reset(new PtrTest);

	TEST_TRUE(pTest1.get() != nullptr);

	pTest1.reset();

	TEST_TRUE(pTest1.get() == nullptr);
	TEST_THROWS(*pTest1);
	TEST_THROWS(pTest1->release());

//	delete pTest1;	// Shouldn't compile.

	TestPtr pBase1(new PtrTest);
	TestPtr pBase2(new PtrTest);
	TestPtr pBase3(new Derived);

	DerivedPtr pDerived1(new Derived);
	DerivedPtr pDerived2(new Derived);
	TestPtr    pBase4(pDerived1);

	UnrelatedPtr pUnrelated;

	pBase2 = pDerived1;
	pBase2 = pDerived1;

	TEST_TRUE(pBase2.get() == pDerived1.get());

	pDerived2 = Core::static_ptr_cast<Derived>(pBase3);

	TEST_TRUE(pDerived2.get() == pBase3.get());

	pDerived2 = Core::dynamic_ptr_cast<Derived>(pBase3);

	TEST_TRUE(pDerived2.get() == pBase3.get());

//	pUnrelated = Core::static_ptr_cast<Unrelated>(pBase1);	// Shouldn't compile.

	pUnrelated = Core::dynamic_ptr_cast<Unrelated>(pBase1);	// Shouldn't compile.

	TEST_TRUE(pUnrelated.get() == nullptr);
}
