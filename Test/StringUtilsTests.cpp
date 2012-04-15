////////////////////////////////////////////////////////////////////////////////
//! \file   StringUtilsTests.cpp
//! \brief  The unit tests for the string utility functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/StringUtils.hpp>
#include <limits>
#include <limits.h>

#ifdef __GNUG__
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

TEST_SET(StringUtils)
{

TEST_CASE("formatting a number returns the same output as printf")
{
	TEST_TRUE(Core::fmt(TXT("%hd"),   std::numeric_limits<short>::min()) == TXT("-32768"));
	TEST_TRUE(Core::fmt(TXT("%hd"),   std::numeric_limits<short>::max()) == TXT("32767"));
	TEST_TRUE(Core::fmt(TXT("%i"),    std::numeric_limits<int>::min())   == TXT("-2147483648"));
	TEST_TRUE(Core::fmt(TXT("%i"),    std::numeric_limits<int>::max())   == TXT("2147483647"));
	TEST_TRUE(Core::fmt(TXT("%I64d"), std::numeric_limits<int64>::min()) == TXT("-9223372036854775808"));
	TEST_TRUE(Core::fmt(TXT("%I64d"), std::numeric_limits<int64>::max()) == TXT("9223372036854775807"));

	TEST_TRUE(Core::fmt(TXT("%-+20d%%"), std::numeric_limits<int>::max())    == TXT("+2147483647         %"));
	TEST_TRUE(Core::fmt(TXT("%+20d%%"),  std::numeric_limits<int>::max())    == TXT("         +2147483647%"));
	TEST_TRUE(Core::fmt(TXT("%#X"),      std::numeric_limits<ushort>::max()) == TXT("0XFFFF"));

	byte* p = 0;

	TEST_TRUE(Core::fmt(TXT("%#p"), --p) == TXT("0XFFFFFFFF"));

	double dSmall = -0.1234567890123456789;
	double dLarge = -123456789012345667890.0;

	TEST_TRUE(Core::fmt(TXT("%.10e"), dSmall) == TXT("-1.2345678901e-001"));
	TEST_TRUE(Core::fmt(TXT("%.10E"), dLarge) == TXT("-1.2345678901E+020"));
	TEST_TRUE(Core::fmt(TXT("%.10f"), dSmall) == TXT("-0.1234567890"));
	TEST_TRUE(Core::fmt(TXT("%.10f"), dLarge) == TXT("-123456789012345670000.0000000000"));
	TEST_TRUE(Core::fmt(TXT("%g"),    dSmall) == TXT("-0.123457"));
	TEST_TRUE(Core::fmt(TXT("%G"),    dLarge) == TXT("-1.23457E+020"));
}
TEST_CASE_END

TEST_CASE("string conversion from mixed to upper and lower case")
{
	const tchar* string = TXT("TeSt StRiNg");
	tstring str(string);

	Core::makeUpper(str);
	TEST_TRUE(str == TXT("TEST STRING"));
	TEST_TRUE(Core::createUpper(string) == TXT("TEST STRING"));

	Core::makeLower(str);
	TEST_TRUE(str == TXT("test string"));
	TEST_TRUE(Core::createLower(string) == TXT("test string"));

	tstring empty;
	const tchar* buffer = empty.data();

	Core::makeUpper(empty);
	TEST_TRUE(empty.data() == buffer);

	Core::makeLower(empty);
	TEST_TRUE(empty.data() == buffer);
}
TEST_CASE_END

TEST_CASE("trim strips spaces, tabs, carrage returns and newlines from the front and back of strings")
{
	TEST_TRUE(Core::trimCopy(TXT(" \t\r\nTEST")) == TXT("TEST"));
	TEST_TRUE(Core::trimCopy(TXT("TEST \t\r\n")) == TXT("TEST"));
	TEST_TRUE(Core::trimCopy(TXT(" \t \r \n ")).empty());
}
TEST_CASE_END

TEST_CASE("skipping whitespace includes spaces, tabs, carrage returns and newlines")
{
	tchar*       test = TXT(" \t\r\n");
	const tchar* end  = test+tstrlen(test);

	tchar* it = Core::skipWhitespace(test, end);
	TEST_TRUE(it == end);
}
TEST_CASE_END

TEST_CASE("a boolean value is formatted and parsed as a '0' or '1' character")
{
	TEST_TRUE(Core::format<bool>(true) == TXT("1"));
	TEST_TRUE(Core::parse<bool>(TXT(" 1 ")) == true);
	TEST_TRUE(Core::format<bool>(false) == TXT("0"));
	TEST_TRUE(Core::parse<bool>(TXT(" 0 ")) == false);

	TEST_THROWS(Core::parse<bool>(TXT("")));
	TEST_THROWS(Core::parse<bool>(TXT("X")));
	TEST_THROWS(Core::parse<bool>(TXT("99")));
}
TEST_CASE_END

TEST_CASE("signed integers can be formatted and parsed")
{
	TEST_TRUE(Core::format<int>(INT_MIN) == TXT("-2147483648"));
	TEST_TRUE(Core::parse<int>(TXT(" -2147483648 ")) == INT_MIN);
	TEST_TRUE(Core::format<int>(INT_MAX) == TXT("2147483647"));
	TEST_TRUE(Core::parse<int>(TXT(" 2147483647 ")) == INT_MAX);

	TEST_THROWS(Core::parse<int>(TXT("")));
	TEST_THROWS(Core::parse<int>(TXT("-2147483649")));
	TEST_THROWS(Core::parse<int>(TXT("2147483648")));
	TEST_THROWS(Core::parse<int>(TXT("1nv4l1d")));
}
TEST_CASE_END

TEST_CASE("unsigned integers can be formatted and parsed")
{
	TEST_TRUE(Core::format<uint>(0) == TXT("0"));
	TEST_TRUE(Core::parse<uint>(TXT(" 0 ")) == 0);
	TEST_TRUE(Core::format<uint>(UINT_MAX) == TXT("4294967295"));
	TEST_TRUE(Core::parse<uint>(TXT(" 4294967295 ")) == UINT_MAX);
	TEST_THROWS(Core::parse<uint>(TXT("")));
//	TEST_THROWS(Core::parse<uint>(TXT("-1"))); // VC++ parses as signed.
	TEST_THROWS(Core::parse<uint>(TXT(" 4294967296 ")));
	TEST_THROWS(Core::parse<uint>(TXT("1nv4l1d")));
}
TEST_CASE_END

TEST_CASE("64-bit signed integers can be formatted and parsed")
{
	TEST_TRUE(Core::format<int64>(_I64_MIN) == TXT("-9223372036854775808"));
	TEST_TRUE(Core::parse<int64>(TXT(" -9223372036854775808 ")) == _I64_MIN);
	TEST_TRUE(Core::format<int64>(_I64_MAX) == TXT("9223372036854775807"));
	TEST_TRUE(Core::parse<int64>(TXT(" 9223372036854775807 ")) == _I64_MAX);
	TEST_THROWS(Core::parse<int64>(TXT("")));
	TEST_THROWS(Core::parse<int64>(TXT("-9223372036854775809")));
	TEST_THROWS(Core::parse<int64>(TXT("9223372036854775808")));
	TEST_THROWS(Core::parse<int64>(TXT("1nv4l1d")));
}
TEST_CASE_END

TEST_CASE("64-bit unsigned integers can be formatted and parsed")
{
	TEST_TRUE(Core::format<uint64>(0) == TXT("0"));
	TEST_TRUE(Core::parse<uint64>(TXT(" 0 ")) == 0);
	TEST_TRUE(Core::format<uint64>(_UI64_MAX) == TXT("18446744073709551615"));
	TEST_TRUE(Core::parse<uint64>(TXT(" 18446744073709551615 ")) == _UI64_MAX);
	TEST_THROWS(Core::parse<uint64>(TXT("")));
//	TEST_THROWS(Core::parse<uint64>(TXT("-1"))); // VC++ parses as signed.
	TEST_THROWS(Core::parse<uint64>(TXT(" 18446744073709551616 ")));
	TEST_THROWS(Core::parse<uint64>(TXT("1nv4l1d")));
}
TEST_CASE_END

TEST_CASE("strnlen should return the string length when less than the buffer size")
{
	const char   buffer[] = "unit test\0garbage";
	const size_t bufsize = ARRAY_SIZE(buffer)-1;
	const size_t length = strlen(buffer);

	const size_t actual = strnlen(buffer, bufsize);

	TEST_TRUE(actual == length);
}
TEST_CASE_END

TEST_CASE("strnlen should return the buffer size when no null terminator exists")
{
	const char   buffer[] = "unit test";
	const size_t bufsize = 4;

	const size_t actual = strnlen(buffer, bufsize);

	TEST_TRUE(actual == bufsize);
}
TEST_CASE_END

TEST_CASE("wcsnlen should return the string length when less than the buffer size")
{
	const wchar_t buffer[] = L"unit test\0garbage";
	const size_t bufsize = ARRAY_SIZE(buffer)-1;
	const size_t length = wcslen(buffer);

	const size_t actual = wcsnlen(buffer, bufsize);

	TEST_TRUE(actual == length);
}
TEST_CASE_END

TEST_CASE("wcsnlen should return the buffer size when no null terminator exists")
{
	const wchar_t buffer[] = L"unit test";
	const size_t bufsize = 4;

	const size_t actual = wcsnlen(buffer, bufsize);

	TEST_TRUE(actual == bufsize);
}
TEST_CASE_END

TEST_CASE("tstrnlen should map to strnlen or wcsnlen")
{
	const tchar  buffer[] = TXT("unit test\0garbage");
	const size_t bufsize = ARRAY_SIZE(buffer);
	const size_t length = tstrlen(buffer);

	const size_t actual = tstrnlen(buffer, bufsize);

	TEST_TRUE(actual == length);
}
TEST_CASE_END

}
TEST_SET_END
