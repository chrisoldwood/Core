////////////////////////////////////////////////////////////////////////////////
//! \file   FileSystemTests.cpp
//! \brief  The unit tests for the filesystem functions.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/FileSystem.hpp>
#include <Core/AnsiWide.hpp>
#include <Core/tfstream.hpp>

static void createEmptyFile(const tstring& path)
{
	tofstream testFile(T2A(path));

	testFile.close();
}

TEST_SET(Path)
{

TEST_CASE("getTempFolder should not return an empty path")
{
	tstring folder = Core::getTempFolder();

	TEST_TRUE(folder.empty() != true);
}
TEST_CASE_END

TEST_CASE("combinePaths should ensure only 1 folder separator exists at the join")
{
	tstring actual = Core::combinePaths(TXT("folder"), TXT("file"));

	TEST_TRUE(actual == TXT("folder\\file"));

	actual = Core::combinePaths(TXT("folder\\"), TXT("file"));

	TEST_TRUE(actual == TXT("folder\\file"));
}
TEST_CASE_END

TEST_CASE("pathExists should return true when the path exists")
{
	TEST_TRUE(Core::pathExists(Core::getTempFolder()));	
}
TEST_CASE_END

TEST_CASE("pathExists should return false when the path is invalid")
{
	TEST_FALSE(Core::pathExists(TXT(".\\invalid_local_file_name.txt")));	
}
TEST_CASE_END

TEST_CASE("deleteFile should succeed when the file exists")
{
	tstring testFileName = Core::combinePaths(Core::getTempFolder(), TXT("core_test_file.txt"));

	createEmptyFile(testFileName);

	ASSERT(Core::pathExists(testFileName));

	Core::deleteFile(testFileName);

	TEST_FALSE(Core::pathExists(testFileName));
}
TEST_CASE_END

TEST_CASE("deleteFile should throw when an error occurs")
{
	tstring invalidFile = TXT(".\\invalid_local_file_name.txt");

	TEST_THROWS(Core::deleteFile(invalidFile));
}
TEST_CASE_END

TEST_CASE("deleteFile should not throw when an error occurs and erros should be ignored")
{
	tstring invalidFile = TXT(".\\invalid_local_file_name.txt");

	Core::deleteFile(invalidFile, true);

	TEST_PASSED("no exception thrown");
}
TEST_CASE_END

}
TEST_SET_END
