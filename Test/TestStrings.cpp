////////////////////////////////////////////////////////////////////////////////
//! \file   TestStrings.cpp
//! \brief  The unit tests for the string conversion functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/AnsiWide.hpp>
#include <Core/StringUtils.hpp>
#include <limits>

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the string conversion functions.

void TestStrings()
{
{
	const char*    psz  =  "ABCabc123";
	const wchar_t* wpsz = L"ABCabc123";
	const tchar*   tpsz = TXT("ABCabc123");

	TEST_TRUE(std::string(W2A(wpsz)) == psz);
	TEST_TRUE(std::wstring(A2W(psz)) == wpsz);

	TEST_TRUE(std::tstring(W2T(wpsz)) == tpsz);
	TEST_TRUE(std::tstring(A2T(psz))  == tpsz);
}
{
	const tchar* tpsz = TXT("[A]NSI [U]NICODE");

#ifdef ANSI_BUILD
	TEST_TRUE(Core::Fmt(TXT("[%c]%s [%C]%S"),      'A',  "NSI", L'U', L"NICODE") == tpsz);
#else
	TEST_TRUE(Core::Fmt(TXT("[%c]%s [%C]%S"),     L'A', L"NSI",  'U',  "NICODE") == tpsz);
#endif
	TEST_TRUE(Core::Fmt(TXT("[%hC]%hS [%wc]%ws"),  'A',  "NSI", L'U', L"NICODE") == tpsz);
}
{
	TEST_TRUE(Core::Fmt(TXT("%hd"),   std::numeric_limits<short>::min()) == TXT("-32768"));
	TEST_TRUE(Core::Fmt(TXT("%hd"),   std::numeric_limits<short>::max()) == TXT("32767"));
	TEST_TRUE(Core::Fmt(TXT("%i"),    std::numeric_limits<int>::min())   == TXT("-2147483648"));
	TEST_TRUE(Core::Fmt(TXT("%i"),    std::numeric_limits<int>::max())   == TXT("2147483647"));
	TEST_TRUE(Core::Fmt(TXT("%I64d"), std::numeric_limits<int64>::min()) == TXT("-9223372036854775808"));
	TEST_TRUE(Core::Fmt(TXT("%I64d"), std::numeric_limits<int64>::max()) == TXT("9223372036854775807"));

	TEST_TRUE(Core::Fmt(TXT("%-+20d%%"), std::numeric_limits<int>::max())    == TXT("+2147483647         %"));
	TEST_TRUE(Core::Fmt(TXT("%+20d%%"),  std::numeric_limits<int>::max())    == TXT("         +2147483647%"));
	TEST_TRUE(Core::Fmt(TXT("%#X"),      std::numeric_limits<ushort>::max()) == TXT("0XFFFF"));

	byte* p = 0;

	TEST_TRUE(Core::Fmt(TXT("%#p"), --p) == TXT("0XFFFFFFFF"));

	double dSmall = -0.1234567890123456789;
	double dLarge = -123456789012345667890.0;

	TEST_TRUE(Core::Fmt(TXT("%.10e"), dSmall) == TXT("-1.2345678901e-001"));
	TEST_TRUE(Core::Fmt(TXT("%.10E"), dLarge) == TXT("-1.2345678901E+020"));
	TEST_TRUE(Core::Fmt(TXT("%.10f"), dSmall) == TXT("-0.1234567890"));
	TEST_TRUE(Core::Fmt(TXT("%.10f"), dLarge) == TXT("-123456789012345670000.0000000000"));
	TEST_TRUE(Core::Fmt(TXT("%g"),    dSmall) == TXT("-0.123457"));
	TEST_TRUE(Core::Fmt(TXT("%G"),    dLarge) == TXT("-1.23457E+020"));
}
}
