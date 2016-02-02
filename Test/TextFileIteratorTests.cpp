////////////////////////////////////////////////////////////////////////////////
//! \file   TextFileIteratorTests.cpp
//! \brief  The unit tests for the TextFileIterator class.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <Core/TextFileIterator.hpp>
#include <Core/AnsiWide.hpp>
#include <Core/tfstream.hpp>
#include <Core/FileSystem.hpp>

static const tchar* s_testLine = TXT("hello world");

static void createEmptyFile(const tstring& path)
{
	tofstream testFile(T2A(path));

	testFile.close();
}

static void createTestTextFile(const tstring& path)
{
	tofstream testFile(T2A(path));

	testFile << s_testLine << std::endl;

	testFile.close();
}

TEST_SET(TextFileIterator)
{
	tstring testEmptyFile = Core::combinePaths(Core::getTempFolder(), TXT("core_empty_test_file.txt"));

	createEmptyFile(testEmptyFile);

	tstring testTextFile = Core::combinePaths(Core::getTempFolder(), TXT("core_test_text_file.txt"));

	createTestTextFile(testTextFile);

TEST_CASE("paramaterless ctor creates an end iterator")
{
	Core::TextFileIterator end;

	TEST_PASSED("object created");
}
TEST_CASE_END

TEST_CASE("end iterator throws when incremented")
{
	Core::TextFileIterator end;

	TEST_THROWS(++end);
}
TEST_CASE_END

TEST_CASE("dereferencing an end iterator should throw an exception")
{
	Core::TextFileIterator end;

	TEST_THROWS(*end);
}
TEST_CASE_END

TEST_CASE("invoking ptr-to-member on an end iterator should throw an exception")
{
	Core::TextFileIterator end;

	TEST_THROWS(end->empty());
}
TEST_CASE_END

TEST_CASE("end iterator compares equal with itself")
{
	Core::TextFileIterator lhs;
	Core::TextFileIterator rhs;

	TEST_TRUE(lhs == rhs);
}
TEST_CASE_END

TEST_CASE("construction from a filename creates an iterator on the file")
{
	Core::TextFileIterator begin(testTextFile);

	TEST_PASSED("object created");
}
TEST_CASE_END

TEST_CASE("construction from an invalid filename throws an exception")
{
	tstring invalidFile = TXT(".\\invalid_local_file_name.txt");

	TEST_THROWS(Core::TextFileIterator(invalidFile));
}
TEST_CASE_END

TEST_CASE("dereferencing the iterator should return the current line")
{
	Core::TextFileIterator it(testTextFile);

	TEST_TRUE(*it == s_testLine);
}
TEST_CASE_END

TEST_CASE("invoking the ptr-to-member on an iterator should return the current line")
{
	Core::TextFileIterator it(testTextFile);

	TEST_FALSE(it->empty());
}
TEST_CASE_END

TEST_CASE("advancing the iterator should eventually reach the end")
{
	Core::TextFileIterator end;
	Core::TextFileIterator it(testTextFile);

	++it;

	TEST_TRUE(it == end);
}
TEST_CASE_END

TEST_CASE("opening an empty file should set the iterator to the end")
{
	Core::TextFileIterator end;
	Core::TextFileIterator it(testEmptyFile);

	TEST_TRUE(it == end);
}
TEST_CASE_END

	Core::deleteFile(testEmptyFile, true);
	Core::deleteFile(testTextFile, true);
}
TEST_SET_END
