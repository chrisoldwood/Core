////////////////////////////////////////////////////////////////////////////////
//! \file   TestPointers.cpp
//! \brief  The unit tests for the smart pointer types.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/IFacePtr.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The test interface.

class IPtrTest
{
public:
	virtual ulong AddRef() = 0;
	virtual ulong Release() = 0;
	virtual void  Run() = 0;
};

////////////////////////////////////////////////////////////////////////////////
//! The test concrete class.

class PtrTest : public IPtrTest
{
public:
	PtrTest()
		: m_nRefCount(1)
	{ }

	virtual ulong AddRef()
	{
		return ++m_nRefCount;
	}

	virtual ulong Release()
	{
		if (--m_nRefCount == 0)
			delete this;
		return m_nRefCount;
	}

	virtual void Run()
	{ }

	ulong m_nRefCount;
};

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the smart pointer types.

void TestPointers()
{
{
	typedef Core::SharedPtr<PtrTest> TestPtr;

	TestPtr pTest1;
//	TestPtr pTest2 = new Test;			// Shouldn't compile.
	TestPtr pTest3 = TestPtr(new PtrTest);
	TestPtr pTest4(new PtrTest);
	TestPtr pTest5(TestPtr(new PtrTest));

	TEST_TRUE(pTest1.Get() == nullptr);
	TEST_TRUE(pTest3.Get() != nullptr);
	TEST_TRUE(pTest4.Get() != nullptr);
	TEST_TRUE(pTest5.Get() != nullptr);

	pTest1 = pTest3;
	pTest1 = pTest1;
//	Test* pRaw1 = pTest1;				// Shouldn't compile.

	TEST_TRUE(pTest1.Get() == pTest3.Get());

	pTest1->Run();
	(*pTest1).Run();

	PtrTest* pRaw2 = pTest1.Get();
	PtrTest& oRef  = pTest1.GetRef();

	TEST_TRUE(pRaw2 == pTest1.Get());
	TEST_TRUE(&oRef == pTest1.Get());

	pRaw2->Run();
	oRef.Run();

//	TEST_FALSE((pTest1 == NULL) || (pTest1 != NULL));	// Shouldn't compile.
	TEST_FALSE(!pTest1);

	TEST_TRUE(pTest1 == pTest3);
	TEST_TRUE(pTest1 != pTest4);

	pTest1.Reset(new PtrTest);

	TEST_TRUE(pTest1.Get() != nullptr);

	pTest1.Reset();

	TEST_TRUE(pTest1.Get() == nullptr);
}
{
	typedef Core::IFacePtr<IPtrTest> ITestPtr;

	ITestPtr pITest1;
//	ITestPtr pITest2 = new Test;			// Shouldn't compile.
	ITestPtr pITest3 = ITestPtr(new PtrTest);
	ITestPtr pITest4(new PtrTest);
	ITestPtr pITest5(ITestPtr(new PtrTest));

	TEST_TRUE(pITest1.Get() == nullptr);
	TEST_TRUE(pITest3.Get() != nullptr);
	TEST_TRUE(pITest4.Get() != nullptr);
	TEST_TRUE(pITest5.Get() != nullptr);

	pITest1 = pITest3;
	pITest1 = pITest1;
//	ITest* pIRaw = pITest1;				// Shouldn't compile.

	TEST_TRUE(pITest1.Get() == pITest3.Get());

	pITest1->Run();
	(*pITest1).Run();

//	TEST_FALSE((pITest1 == NULL) || (pITest1 != NULL));	// Shouldn't compile.
	TEST_FALSE(!pITest1);

	TEST_TRUE(pITest1 == pITest3);
	TEST_TRUE(pITest1 != pITest4);

	pITest1.Release();

	TEST_TRUE(pITest1.Get() == nullptr);

	delete pITest4.Detach();

	TEST_TRUE(pITest4.Get() == nullptr);
}
}
