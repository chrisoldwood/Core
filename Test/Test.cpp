////////////////////////////////////////////////////////////////////////////////
//

#include "stdafx.h"
#include <Core/IFacePtr.hpp>
#include <Core/AnsiWide.hpp>
#include <Core/StringUtils.hpp>
#include <limits>

class LeakCheck
{
public:
	~LeakCheck()
	{ _CrtDumpMemoryLeaks(); }

} g_oChecker;

typedef unsigned long ulong;

////////////////////////////////////////////////////////////////////////////////
//

class ITest
{
public:
	virtual ulong AddRef() = 0;
	virtual ulong Release() = 0;
	virtual void  Run() = 0;
};

class Test : public ITest, private Core::NotCopyable
{
public:
	Test()
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
//

void TestDebug()
{
	bool bTrue  = true;
//	bool bFalse = false;

	ASSERT(bTrue);

//	ASSERT(bFalse);

	STATIC_ASSERT(true);

//	STATIC_ASSERT(false);

	TRACE1("Test TraceEx(\"%s\")\n", "Hello World");

	Core::DebugWrite("Test DebugWrite(\"%s\")\n", "Hello World");
}

////////////////////////////////////////////////////////////////////////////////
//

void TestUtils()
{
	{
	Test oTest1;
//	Test oTest2(oTest1);	// Shouldn't compile.
	Test oTest3;
//	oTest3 = oTest1;		// Shouldn't compile.
	}

	{
	typedef Core::SharedPtr<Test> TestPtr;

	TestPtr pTest1;
//	TestPtr pTest2 = new Test;			// Shouldn't compile.
	TestPtr pTest3 = TestPtr(new Test);
	TestPtr pTest4(new Test);
	TestPtr pTest5(TestPtr(new Test));

	pTest1 = pTest3;
	pTest1 = pTest1;
//	Test* pRaw = pTest1;				// Shouldn't compile.

	pTest1->Run();
	(*pTest1).Run();

	Test* pRaw = pTest1.Get();
	Test& oRef = pTest1.GetRef();

	pRaw->Run();
	oRef.Run();

//	if ( (pTest1 == NULL) || (pTest1 != NULL) )	// Shouldn't compile.
//		__asm nop;

	if ( (!pTest1) || (pTest1.Get() == nullptr))
		__asm nop;

	if ( (pTest1 == pTest3) && (pTest1 != pTest4) )
		__asm nop;

	pTest1.Reset(new Test);
	}

	{
	typedef Core::IFacePtr<ITest> ITestPtr;

	ITestPtr pITest1;
//	ITestPtr pITest2 = new Test;			// Shouldn't compile.
	ITestPtr pITest3 = ITestPtr(new Test);
	ITestPtr pITest4(new Test);
	ITestPtr pITest5(ITestPtr(new Test));

	pITest1 = pITest3;
	pITest1 = pITest1;
//	ITest* pIRaw = pITest1;				// Shouldn't compile.

	pITest1->Run();
	(*pITest1).Run();

//	if ( (pITest1 == NULL) || (pITest1 != NULL) )	// Shouldn't compile.
//		__asm nop;

	if ( (!pITest1) || (pITest1.Get() == nullptr))
		__asm nop;

	if ( (pITest1 == pITest3) && (pITest1 != pITest4) )
		__asm nop;

	pITest1.Release();
	delete pITest4.Detach();
	}
}

////////////////////////////////////////////////////////////////////////////////
//

void TestStrings()
{
	const char*    psz  =  "ABCabc123";
	const wchar_t* wpsz = L"ABCabc123";

	std::cout  << W2A(wpsz) << std::endl;
	std::wcout << A2W(psz)  << std::endl;

	std::tcout << W2T(wpsz) << std::endl;
	std::tcout << A2T(psz)  << std::endl;

	std::tcout << Core::Fmt("[%c]%s [%C]%S\n",     'A', "NSI", L'U', L"NICODE");
	std::tcout << Core::Fmt("[%hC]%hS [%wc]%ws\n", 'A', "NSI", L'U', L"NICODE");

	std::tcout << Core::Fmt("%hd\n",   std::numeric_limits<short>::min());
	std::tcout << Core::Fmt("%hd\n",   std::numeric_limits<short>::max());
	std::tcout << Core::Fmt("%i\n",    std::numeric_limits<int>::min());
	std::tcout << Core::Fmt("%i\n",    std::numeric_limits<int>::max());
	std::tcout << Core::Fmt("%I64d\n", std::numeric_limits<int64>::min());
	std::tcout << Core::Fmt("%I64d\n", std::numeric_limits<int64>::max());

	std::tcout << Core::Fmt("%-+20d%%\n", std::numeric_limits<int>::max());
	std::tcout << Core::Fmt("%+20d%%\n",  std::numeric_limits<int>::max());
	std::tcout << Core::Fmt("%#X\n",      std::numeric_limits<ushort>::max());

	byte* p = 0;

	std::tcout << Core::Fmt("%#p\n", --p);

	double dSmall = -0.1234567890123456789;
	double dLarge = -123456789012345667890.0;

	std::tcout << Core::Fmt("%.10e\n", dSmall);
	std::tcout << Core::Fmt("%.10E\n", dLarge);
	std::tcout << Core::Fmt("%.10f\n", dSmall);
	std::tcout << Core::Fmt("%.10f\n", dLarge);
	std::tcout << Core::Fmt("%g\n", dSmall);
	std::tcout << Core::Fmt("%G\n", dLarge);
}

////////////////////////////////////////////////////////////////////////////////
//

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	try
	{
		TestDebug();

		TestUtils();

		TestStrings();
	}
	catch (const std::exception& e)
	{
		std::tcout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
