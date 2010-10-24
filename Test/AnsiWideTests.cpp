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

TEST_CASE("functions")
{
	TEST_TRUE(Core::ansiToWide(psz,  psz+strlen(psz)  ) == wpsz);
	TEST_TRUE(Core::wideToAnsi(wpsz, wpsz+wcslen(wpsz)) == psz);
}
TEST_CASE_END

TEST_CASE("classes")
{
	TEST_TRUE(static_cast<const char*>(Core::WideToAnsi(std::wstring(wpsz))) == std::string(psz));
	TEST_TRUE(static_cast<const wchar_t*>(Core::AnsiToWide(std::string(psz)))  == std::wstring(wpsz));
}
TEST_CASE_END

TEST_CASE("macros")
{
	TEST_TRUE(std::string(W2A(wpsz)) == psz);
	TEST_TRUE(std::wstring(A2W(psz)) == wpsz);

	TEST_TRUE(tstring(W2T(wpsz)) == tpsz);
	TEST_TRUE(tstring(A2T(psz))  == tpsz);
}
TEST_CASE_END

TEST_CASE("fmt")
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
