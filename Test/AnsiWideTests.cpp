////////////////////////////////////////////////////////////////////////////////
//! \file   AnsiWideTests.cpp
//! \brief  The unit tests for the ansi/wide string conversion functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/AnsiWide.hpp>

TEST_SET(AnsiWide)
{
	const char*    psz  =  "ABCabc123";
	const wchar_t* wpsz = L"ABCabc123";
	const tchar*   tpsz = TXT("ABCabc123");

TEST_CASE("convert a string from ANSI to Unicode")
{
	const std::string  test = psz;
	const std::wstring expected = wpsz;

	TEST_TRUE(Core::ansiToWide(test) == expected);
	TEST_TRUE(Core::ansiToWide(test.c_str()) == expected);
	TEST_TRUE(Core::ansiToWide(test.c_str(), test.c_str()+test.size()) == expected);

	TEST_TRUE(static_cast<const wchar_t*>(Core::AnsiToWide(test.c_str())) == expected);
	TEST_TRUE(static_cast<const wchar_t*>(Core::AnsiToWide(test)) == expected);

	TEST_TRUE(A2W(test.c_str()) == expected);
}
TEST_CASE_END

TEST_CASE("convert a string from Unicode to ANSI")
{
	const std::wstring test = wpsz;
	const std::string  expected = psz;

	TEST_TRUE(Core::wideToAnsi(test) == expected);
	TEST_TRUE(Core::wideToAnsi(test.c_str()) == expected);
	TEST_TRUE(Core::wideToAnsi(test.c_str(), test.c_str()+test.size()) == expected);

	TEST_TRUE(static_cast<const char*>(Core::WideToAnsi(test.c_str())) == expected);
	TEST_TRUE(static_cast<const char*>(Core::WideToAnsi(test)) == expected);

	TEST_TRUE(W2A(test.c_str()) == expected);
}
TEST_CASE_END

TEST_CASE("convert from ANSI/Unicode to build dependent type")
{
	tstring expected = tpsz;

	TEST_TRUE(A2T(psz)  == expected);
	TEST_TRUE(W2T(wpsz) == expected);
}
TEST_CASE_END

TEST_CASE("convert from build dependent type to ANSI/Unicode")
{
	std::string  ansiExpected = psz;
	std::wstring wideExpected = wpsz;

	TEST_TRUE(T2A(tpsz) == ansiExpected);
	TEST_TRUE(T2W(tpsz) == wideExpected);
}
TEST_CASE_END

TEST_CASE("format a string from ANSI, Unicode or build dependent input strings")
{
	const tchar* result = TXT("[A]NSI [U]NICODE");

#ifdef ANSI_BUILD
	TEST_TRUE(Core::fmt(TXT("[%c]%s [%C]%S"),      'A',  "NSI", L'U', L"NICODE") == result);
#else
	TEST_TRUE(Core::fmt(TXT("[%c]%s [%C]%S"),     L'A', L"NSI",  'U',  "NICODE") == result);
#endif
	TEST_TRUE(Core::fmt(TXT("[%hC]%hS [%wc]%ws"),  'A',  "NSI", L'U', L"NICODE") == result);
}
TEST_CASE_END

}
TEST_SET_END
