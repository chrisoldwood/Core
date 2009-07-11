////////////////////////////////////////////////////////////////////////////////
//! \file   TestUniquePtr.cpp
//! \brief  The unit tests for the UniquePtr class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/UniquePtr.hpp>
#include "PtrTest.hpp"

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the UniquePtr class.

void testUniquePtr()
{
	typedef Core::UniquePtr<PtrTest> TestPtr;

	TestPtr pTest1;
//	TestPtr pTest2 = new PtrTest;			// Shouldn't compile.
//	TestPtr pTest3 = TestPtr(new PtrTest);	// Shouldn't compile.
	TestPtr pTest4(new PtrTest);
//	TestPtr pTest5(pTest4);					// Shouldn't compile.
	TestPtr pTest6;

	TEST_TRUE(pTest1.get() == nullptr);
	TEST_TRUE(pTest4.get() != nullptr);

//	pTest1 = pTest4;			// Shouldn't compile.
//	PtrTest* pRaw1 = pTest1;	// Shouldn't compile.

	pTest4->run();
	(*pTest4).run();

	PtrTest* pRaw2 = pTest4.get();
	PtrTest& oRef  = pTest4.getRef();

	TEST_TRUE(pRaw2 == pTest4.get());
	TEST_TRUE(&oRef == pTest4.get());

	pRaw2->run();
	oRef.run();

//	TEST_FALSE((pTest1 == NULL) || (pTest1 != NULL));	// Shouldn't compile.
	TEST_FALSE(!pTest4);

	TEST_TRUE(pTest1 == pTest6);
	TEST_TRUE(pTest1 != pTest4);

	pTest1.reset(new PtrTest);

	TEST_TRUE(pTest1.get() != nullptr);

	pTest1.reset();

	TEST_TRUE(pTest1.get() == nullptr);

	TEST_THROWS(*pTest1);
	TEST_THROWS(pTest1->release());

	delete pTest4.detach();

	TEST_TRUE(pTest4.get() == nullptr);

//	delete pTest1;	// Shouldn't compile.

	TEST_TRUE(*attachTo(pTest4) == nullptr);
}
