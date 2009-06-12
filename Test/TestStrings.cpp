////////////////////////////////////////////////////////////////////////////////
//! \file   TestStrings.cpp
//! \brief  The unit tests for the string conversion functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/AnsiWide.hpp>
#include <Core/StringUtils.hpp>
#include <limits>
#include <limits.h>

#ifdef __GNUG__
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

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

	TEST_TRUE(tstring(W2T(wpsz)) == tpsz);
	TEST_TRUE(tstring(A2T(psz))  == tpsz);
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
{
	const tchar* string = TXT("TeSt StRiNg");
	tstring str(string);

	Core::makeUpper(str);
	TEST_TRUE(str == TXT("TEST STRING"));
	TEST_TRUE(Core::createUpper(string) == TXT("TEST STRING"));

	Core::makeLower(str);
	TEST_TRUE(str == TXT("test string"));
	TEST_TRUE(Core::createLower(string) == TXT("test string"));
}
{
	tchar*       test = TXT(" \t\r\n");
	const tchar* end  = test+tstrlen(test);

	tchar* it = Core::skipWhitespace(test, end);
	TEST_TRUE(it == end);
}
{
	TEST_TRUE(Core::format<uint>(0) == TXT("0"));
	TEST_TRUE(Core::parse<uint>(TXT(" 0 ")) == 0);

	TEST_TRUE(Core::format<uint>(UINT_MAX) == TXT("4294967295"));
	TEST_TRUE(Core::parse<uint>(TXT(" 4294967295 ")) == UINT_MAX);

	TEST_THROWS(Core::parse<uint>(TXT("")));
//	TEST_THROWS(Core::parse<uint>(TXT("-1"))); // VC++ parses as signed.
	TEST_THROWS(Core::parse<uint>(TXT(" 4294967296 ")));
	TEST_THROWS(Core::parse<uint>(TXT("1nv4l1d")));

	TEST_TRUE(Core::format<int>(INT_MIN) == TXT("-2147483648"));
	TEST_TRUE(Core::parse<int>(TXT(" -2147483648 ")) == INT_MIN);

	TEST_TRUE(Core::format<int>(INT_MAX) == TXT("2147483647"));
	TEST_TRUE(Core::parse<int>(TXT(" 2147483647 ")) == INT_MAX);

	TEST_THROWS(Core::parse<int>(TXT("")));
	TEST_THROWS(Core::parse<int>(TXT("-2147483649")));
	TEST_THROWS(Core::parse<int>(TXT("2147483648")));
	TEST_THROWS(Core::parse<int>(TXT("1nv4l1d")));

	TEST_TRUE(Core::format<bool>(true) == TXT("1"));
	TEST_TRUE(Core::parse<bool>(TXT(" 1 ")) == true);
	TEST_TRUE(Core::format<bool>(false) == TXT("0"));
	TEST_TRUE(Core::parse<bool>(TXT(" 0 ")) == false);
	TEST_THROWS(Core::parse<bool>(TXT("")));
	TEST_THROWS(Core::parse<bool>(TXT("X")));
}
}
