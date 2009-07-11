////////////////////////////////////////////////////////////////////////////////
//! \file   TestArrayPtr.cpp
//! \brief  The unit tests for the ArrayPtr class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/ArrayPtr.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the ArrayPtr class.

void testArrayPtr()
{
	typedef Core::ArrayPtr<int> TestPtr;

	TestPtr pTest1;
//	TestPtr pTest2 = new int[1];			// Shouldn't compile.
//	TestPtr pTest3 = TestPtr(new int[1]);	// Shouldn't compile.
	TestPtr pTest4(new int[1]);
//	TestPtr pTest5(pTest4);					// Shouldn't compile.
	TestPtr pTest6;

	TEST_TRUE(pTest1.get() == nullptr);
	TEST_TRUE(pTest4.get() != nullptr);

//	pTest1 = pTest4;			// Shouldn't compile.
//	int pRaw1[1] = pTest1;		// Shouldn't compile.

	int* pRaw2 = pTest4.get();
	int& oRef  = pTest4.getRef();

	TEST_TRUE(pRaw2 == pTest4.get());
	TEST_TRUE(&oRef == pTest4.get());

//	TEST_FALSE((pTest1 == NULL) || (pTest1 != NULL));	// Shouldn't compile.
	TEST_FALSE(!pTest4);

	TEST_TRUE(pTest1 == pTest6);
	TEST_TRUE(pTest1 != pTest4);

	int aiTest[1] = { 12345678 };

	pTest1.reset(aiTest);

	TEST_TRUE(pTest1.get() != nullptr);

	TEST_TRUE(pTest1[0] == 12345678);

	pTest1.detach();

	TEST_TRUE(pTest1.get() == nullptr);

	TEST_THROWS(*pTest1);
	TEST_THROWS(pTest1[0]);

	delete[] pTest4.detach();

	TEST_TRUE(pTest4.get() == nullptr);

//	delete[] pTest1;	// Shouldn't compile.

	TEST_TRUE(*attachTo(pTest4) == nullptr);
}
