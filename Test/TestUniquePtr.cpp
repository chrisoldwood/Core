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

void TestUniquePtr()
{
	typedef Core::UniquePtr<PtrTest> TestPtr;

	TestPtr pTest1;
//	TestPtr pTest2 = new PtrTest;			// Shouldn't compile.
//	TestPtr pTest3 = TestPtr(new PtrTest);	// Shouldn't compile.
	TestPtr pTest4(new PtrTest);
//	TestPtr pTest5(pTest4);					// Shouldn't compile.
	TestPtr pTest6;

	TEST_TRUE(pTest1.Get() == nullptr);
	TEST_TRUE(pTest4.Get() != nullptr);

//	pTest1 = pTest4;			// Shouldn't compile.
//	PtrTest* pRaw1 = pTest1;	// Shouldn't compile.

	pTest4->Run();
	(*pTest4).Run();

	PtrTest* pRaw2 = pTest4.Get();
	PtrTest& oRef  = pTest4.GetRef();

	TEST_TRUE(pRaw2 == pTest4.Get());
	TEST_TRUE(&oRef == pTest4.Get());

	pRaw2->Run();
	oRef.Run();

//	TEST_FALSE((pTest1 == NULL) || (pTest1 != NULL));	// Shouldn't compile.
	TEST_FALSE(!pTest4);

	TEST_TRUE(pTest1 == pTest6);
	TEST_TRUE(pTest1 != pTest4);

	pTest1.Reset(new PtrTest);

	TEST_TRUE(pTest1.Get() != nullptr);

	pTest1.Reset();

	TEST_TRUE(pTest1.Get() == nullptr);

	TEST_THROWS(*pTest1);
	TEST_THROWS(pTest1->Release());

	delete pTest4.Detach();

	TEST_TRUE(pTest4.Get() == nullptr);

//	delete pTest1;	// Shouldn't compile.
}
