////////////////////////////////////////////////////////////////////////////////
//! \file   TestArrayPtr.cpp
//! \brief  The unit tests for the ArrayPtr class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/ArrayPtr.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the ArrayPtr class.

void TestArrayPtr()
{
	typedef Core::ArrayPtr<int> TestPtr;

	TestPtr pTest1;
//	TestPtr pTest2 = new int[1];			// Shouldn't compile.
//	TestPtr pTest3 = TestPtr(new int[1]);	// Shouldn't compile.
	TestPtr pTest4(new int[1]);
//	TestPtr pTest5(pTest4);					// Shouldn't compile.
	TestPtr pTest6;

	TEST_TRUE(pTest1.Get() == nullptr);
	TEST_TRUE(pTest4.Get() != nullptr);

//	pTest1 = pTest4;			// Shouldn't compile.
//	int pRaw1[1] = pTest1;		// Shouldn't compile.

	int* pRaw2 = pTest4.Get();
	int& oRef  = pTest4.GetRef();

	TEST_TRUE(pRaw2 == pTest4.Get());
	TEST_TRUE(&oRef == pTest4.Get());

//	TEST_FALSE((pTest1 == NULL) || (pTest1 != NULL));	// Shouldn't compile.
	TEST_FALSE(!pTest4);

	TEST_TRUE(pTest1 == pTest6);
	TEST_TRUE(pTest1 != pTest4);

	int aiTest[1] = { 12345678 };

	pTest1.Reset(aiTest);

	TEST_TRUE(pTest1.Get() != nullptr);

	TEST_TRUE(pTest1[0] == 12345678);

	pTest1.Detach();

	TEST_TRUE(pTest1.Get() == nullptr);

	TEST_THROWS(*pTest1);
	TEST_THROWS(pTest1[0]);

	delete[] pTest4.Detach();

	TEST_TRUE(pTest4.Get() == nullptr);

//	delete[] pTest1;	// Shouldn't compile.
}
