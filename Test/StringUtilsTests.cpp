////////////////////////////////////////////////////////////////////////////////
//! \file   StringUtilsTests.cpp
//! \brief  The unit tests for the string utility functions.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <Core/StringUtils.hpp>
#include <limits>
#include <limits.h>

#ifdef __GNUG__
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

TEST_SET(StringUtils)
{
#if (_MSC_VER < 1900)
	_set_output_format(_TWO_DIGIT_EXPONENT);
#endif

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

#ifndef __WIN64
	TEST_TRUE(Core::fmt(TXT("%p"), --p) == TXT("FFFFFFFF"));
#else
	TEST_TRUE(Core::fmt(TXT("%p"), --p) == TXT("FFFFFFFFFFFFFFFF"));
#endif

	double small = -0.1234567890123456789;
	double large = -123456789012345667890.0;

	TEST_TRUE(Core::fmt(TXT("%.10e"), small) == TXT("-1.2345678901e-01"));
	TEST_TRUE(Core::fmt(TXT("%.10E"), large) == TXT("-1.2345678901E+20"));
	TEST_TRUE(Core::fmt(TXT("%.10f"), small) == TXT("-0.1234567890"));
#if (_MSC_VER >= 1900)
	TEST_TRUE(Core::fmt(TXT("%.10f"), large) == TXT("-123456789012345667584.0000000000"));
#else
	TEST_TRUE(Core::fmt(TXT("%.10f"), large) == TXT("-123456789012345670000.0000000000"));
#endif
	TEST_TRUE(Core::fmt(TXT("%g"),    small) == TXT("-0.123457"));
	TEST_TRUE(Core::fmt(TXT("%G"),    large) == TXT("-1.23457E+20"));
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
	TEST_THROWS(Core::parse<uint>(TXT("-1")));
	TEST_THROWS(Core::parse<uint>(TXT(" 4294967296 ")));
	TEST_THROWS(Core::parse<uint>(TXT("1nv4l1d")));
}
TEST_CASE_END

TEST_CASE("signed long integers can be formatted and parsed")
{
	TEST_TRUE(Core::format<long>(LONG_MIN) == TXT("-2147483648"));
	TEST_TRUE(Core::parse<long>(TXT(" -2147483648 ")) == LONG_MIN);
	TEST_TRUE(Core::format<long>(LONG_MAX) == TXT("2147483647"));
	TEST_TRUE(Core::parse<long>(TXT(" 2147483647 ")) == LONG_MAX);

	TEST_THROWS(Core::parse<long>(TXT("")));
	TEST_THROWS(Core::parse<long>(TXT("-2147483649")));
	TEST_THROWS(Core::parse<long>(TXT("2147483648")));
	TEST_THROWS(Core::parse<long>(TXT("1nv4l1d")));
}
TEST_CASE_END

TEST_CASE("unsigned long integers can be formatted and parsed")
{
	TEST_TRUE(Core::format<ulong>(0) == TXT("0"));
	TEST_TRUE(Core::parse<ulong>(TXT(" 0 ")) == 0);
	TEST_TRUE(Core::format<ulong>(ULONG_MAX) == TXT("4294967295"));
	TEST_TRUE(Core::parse<ulong>(TXT(" 4294967295 ")) == ULONG_MAX);
	TEST_THROWS(Core::parse<ulong>(TXT("")));
	TEST_THROWS(Core::parse<ulong>(TXT("-1")));
	TEST_THROWS(Core::parse<ulong>(TXT(" 4294967296 ")));
	TEST_THROWS(Core::parse<ulong>(TXT("1nv4l1d")));
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
	TEST_THROWS(Core::parse<uint64>(TXT("-1"))); // VC++ parses as signed.
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

TEST_CASE("left should return the first N characters of the string")
{
	const tstring input = TXT("unit test");
	const tstring expected = TXT("unit");

	const tstring actual = Core::left(input, 4);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("left should return the original input string when too many characters are requested")
{
	const tstring input = TXT("unit test");
	const tstring expected = input;

	const tstring actual = Core::left(input, input.length()+1);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("right should return the last N characters of the string")
{
	const tstring input = TXT("unit test");
	const tstring expected = TXT("test");

	const tstring actual = Core::right(input, 4);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("right should return the original input string when too many characters are requested")
{
	const tstring input = TXT("unit test");
	const tstring expected = input;

	const tstring actual = Core::right(input, input.length()+1);

	TEST_TRUE(actual == expected);
}
TEST_CASE_END

TEST_CASE("tstristr should return NULL when the source string is empty")
{
	const tstring string = TXT("");
	const tstring search = TXT("search");
	const tchar*  expected = nullptr;

	TEST_TRUE(Core::tstristr(string, search) == expected);
}
TEST_CASE_END

TEST_CASE("tstristr should return the source string when the search string is empty")
{
	const tstring string = TXT("string");
	const tstring search = TXT("");
	const tchar*  expected = &string[0];

	TEST_TRUE(Core::tstristr(string, search) == expected);
}
TEST_CASE_END

TEST_CASE("tstristr should return NULL when no match found")
{
	const tstring string = TXT("unit test");
	const tstring search = TXT("wont match");
	const tchar*  expected = nullptr;

	TEST_TRUE(Core::tstristr(string, search) == expected);
}
TEST_CASE_END

TEST_CASE("tstristr should return a pointer to the first matching character")
{
	const tstring string = TXT("unit test");
	const tstring search = TXT("test");
	const tchar*  expected = &string[0] + tstrlen(TXT("unit "));

	TEST_TRUE(Core::tstristr(string, search) == expected);
}
TEST_CASE_END

TEST_CASE("tstristr should match the first occurence")
{
	const tstring string = TXT("match match");
	const tstring search = TXT("match");
	const tchar*  expected = &string[0];

	TEST_TRUE(Core::tstristr(string, search) == expected);
}
TEST_CASE_END

TEST_CASE("tstristr should ignore partial matches")
{
	TEST_TRUE(Core::tstristr(TXT("tes"), TXT("test")) == nullptr);

	TEST_TRUE(Core::tstristr(TXT("tes "), TXT("test")) == nullptr);
}
TEST_CASE_END

TEST_CASE("tstristr should ignore case when searching")
{
	const tstring string = TXT("UniT TesT");
	const tstring search = TXT("test");
	const tchar*  expected = &string[0] + tstrlen(TXT("UniT "));

	TEST_TRUE(Core::tstristr(string, search) == expected);
}
TEST_CASE_END

TEST_CASE("replace should return an empty string when the input string is empty")
{
	const tstring string = TXT("");
	const tstring pattern = TXT("match");
	const tstring replacement = TXT("replacement");
	const tstring expected = TXT("");

	TEST_TRUE(Core::replace(string, pattern, replacement) == expected);
}
TEST_CASE_END

TEST_CASE("replace should return the input string when the pattern is empty")
{
	const tstring string = TXT(" match match ");
	const tstring pattern = TXT("");
	const tstring replacement = TXT("replacement");
	const tstring expected = string;

	TEST_TRUE(Core::replace(string, pattern, replacement) == expected);
}
TEST_CASE_END

TEST_CASE("replace should replace all occurences of the pattern")
{
	const tstring string = TXT(" match match ");
	const tstring pattern = TXT("match");
	const tstring replacement = TXT("replacement");
	const tstring expected = TXT(" replacement replacement ");

	TEST_TRUE(Core::replace(string, pattern, replacement) == expected);
}
TEST_CASE_END

TEST_CASE("replace should handle adjacent matches with no leading or trailing text")
{
	const tstring string = TXT("matchmatch");
	const tstring pattern = TXT("match");
	const tstring replacement = TXT("replacement");
	const tstring expected = TXT("replacementreplacement");

	TEST_TRUE(Core::replace(string, pattern, replacement) == expected);
}
TEST_CASE_END

TEST_CASE("replaceNoCase should ignore the case of the pattern")
{
	const tstring string = TXT(" MatcH maTch ");
	const tstring pattern = TXT("match");
	const tstring replacement = TXT("replacement");
	const tstring expected = TXT(" replacement replacement ");

	TEST_TRUE(Core::replaceNoCase(string, pattern, replacement) == expected);
}
TEST_CASE_END

}
TEST_SET_END
