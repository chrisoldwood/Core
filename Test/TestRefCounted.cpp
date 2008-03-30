////////////////////////////////////////////////////////////////////////////////
//! \file   TestRefCounted.cpp
//! \brief  The unit tests for the RefCounted class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/RefCounted.hpp>
#include <Core/RefCntPtr.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The test ref counted class.

class RefCntTest : public Core::RefCounted
{
public:
	virtual void Run()
	{ }
};

////////////////////////////////////////////////////////////////////////////////
//! The test derived concrete class.

class RefCntDerived : public RefCntTest
{
};

////////////////////////////////////////////////////////////////////////////////
//! The test unrelated concrete class.

class RefCntUnrelated : public Core::RefCounted
{
};

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the RefCounted class.

void TestRefCounted()
{
//	Core::RefCounted	oTest1;				// Shouldn't compile.
	Core::RefCounted*	pTest1 = nullptr;
//	Core::RefCounted	oTest2(*pTest1);	// Shouldn't compile.

	pTest1 = new RefCntTest;

	TEST_TRUE(pTest1->RefCount() == 1);
	TEST_TRUE(pTest1->IncRefCount() == 2);
	TEST_TRUE(pTest1->DecRefCount() == 1);
	TEST_TRUE(pTest1->DecRefCount() == 0);
}

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the RefCntPtr class.

void TestRefCntPtr()
{
	typedef Core::RefCntPtr<RefCntTest> TestPtr;
	typedef Core::RefCntPtr<RefCntDerived> DerivedPtr;
	typedef Core::RefCntPtr<RefCntUnrelated> UnrelatedPtr;

	TestPtr pTest1;
//	TestPtr pTest2 = new RefCntTest;			// Shouldn't compile.
	TestPtr pTest3 = TestPtr(new RefCntTest);
	TestPtr pTest4(new RefCntTest);
	TestPtr pTest5(pTest3);

	TEST_TRUE(pTest1.Get() == nullptr);
	TEST_TRUE(pTest3.Get() != nullptr);
	TEST_TRUE(pTest4.Get() != nullptr);
	TEST_TRUE(pTest5.Get() != nullptr);

	pTest1 = pTest3;
	pTest1 = pTest1;
//	RefCntTest* pRaw1 = pTest1;				// Shouldn't compile.

	TEST_TRUE(pTest1.Get() == pTest3.Get());

	pTest1->Run();
	(*pTest1).Run();

	RefCntTest* pRaw2 = pTest1.Get();
	RefCntTest& oRef  = pTest1.GetRef();

	TEST_TRUE(pRaw2 == pTest1.Get());
	TEST_TRUE(&oRef == pTest1.Get());

	pRaw2->Run();
	oRef.Run();

//	TEST_FALSE((pTest1 == NULL) || (pTest1 != NULL));	// Shouldn't compile.
	TEST_FALSE(!pTest1);

	TEST_TRUE(pTest1 == pTest3);
	TEST_TRUE(pTest1 != pTest4);

	pTest1.Reset(new RefCntTest);

	TEST_TRUE(pTest1.Get() != nullptr);

	pTest1.Reset();

	TEST_TRUE(pTest1.Get() == nullptr);
	TEST_THROWS(*pTest1);
	TEST_THROWS(pTest1->DecRefCount());

//	delete pTest1;	// Shouldn't compile.

	TestPtr pBase1(new RefCntTest);
	TestPtr pBase2(new RefCntTest);
	TestPtr pBase3(new RefCntDerived);

	DerivedPtr pDerived1(new RefCntDerived);
	DerivedPtr pDerived2(new RefCntDerived);
	TestPtr    pBase4(pDerived1);

	UnrelatedPtr pUnrelated;

	pBase2 = pDerived1;
	pBase2 = pDerived1;

	TEST_TRUE(pBase2.Get() == pDerived1.Get());

	pDerived2 = Core::static_ptr_cast<RefCntDerived>(pBase3);

	TEST_TRUE(pDerived2.Get() == pBase3.Get());

	pDerived2 = Core::dynamic_ptr_cast<RefCntDerived>(pBase3);

	TEST_TRUE(pDerived2.Get() == pBase3.Get());

//	pUnrelated = Core::static_ptr_cast<RefCntUnrelated>(pBase1);	// Shouldn't compile.

	pUnrelated = Core::dynamic_ptr_cast<RefCntUnrelated>(pBase1);

	TEST_TRUE(pUnrelated.Get() == nullptr);
}
