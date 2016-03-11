////////////////////////////////////////////////////////////////////////////////
//! \file   FileSystemTests.cpp
//! \brief  The unit tests for the file-system functions.
//! \author Chris Oldwood

#include "Common.hpp"
#include <Core/UnitTest.hpp>
#include <Core/FileSystem.hpp>
#include <Core/AnsiWide.hpp>
#include <Core/tfstream.hpp>

static void createEmptyFile(const tstring& path)
{
	tofstream testFile(T2A(path));

	testFile.close();
}

TEST_SET(FileSystem)
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

TEST_CASE("deleteFile should not throw when an error occurs and errors should be ignored")
{
	tstring invalidFile = TXT(".\\invalid_local_file_name.txt");

	Core::deleteFile(invalidFile, true);

	TEST_PASSED("no exception thrown");
}
TEST_CASE_END

TEST_CASE("creating a folder succeeds when it does not already exist")
{
	const tstring tempFolder = Core::getTempFolder();
	const tstring testFolder = Core::combinePaths(tempFolder, TXT("FileSystemTest"));

	ASSERT(!Core::pathExists(testFolder));

	Core::createFolder(testFolder);

	TEST_TRUE(Core::pathExists(testFolder));

	Core::deleteFolder(testFolder, true);
	ASSERT(!Core::pathExists(testFolder));
}
TEST_CASE_END

TEST_CASE("creating a folder throws when the path already exists")
{
	const tstring tempFolder = Core::getTempFolder();
	const tstring testFolder = Core::combinePaths(tempFolder, TXT("FileSystemTest"));

	ASSERT(!Core::pathExists(testFolder));

	Core::createFolder(testFolder);

	TEST_THROWS(Core::createFolder(testFolder));

	Core::deleteFolder(testFolder, true);
	ASSERT(!Core::pathExists(testFolder));
}
TEST_CASE_END

TEST_CASE("deleting a folder throws when an error occurs")
{
	const tstring invalidFolder = TXT(".\\invalid_local_folder_name");

	TEST_THROWS(Core::deleteFolder(invalidFolder));
}
TEST_CASE_END

TEST_CASE("deleting a folder succeeds when the path exists and references a folder")
{
	const tstring tempFolder = Core::getTempFolder();
	const tstring testFolder = Core::combinePaths(tempFolder, TXT("FileSystemTest"));

	ASSERT(!Core::pathExists(testFolder));
	Core::createFolder(testFolder);

	Core::deleteFolder(testFolder);

	TEST_FALSE(Core::pathExists(testFolder));
}
TEST_CASE_END

TEST_CASE("deleting a folder doesn't throw when an error occurs and errors should be ignored")
{
	const tstring invalidFolder = TXT(".\\invalid_local_folder_name");

	Core::deleteFolder(invalidFolder, true);

	TEST_PASSED("no exception thrown");
}
TEST_CASE_END

}
TEST_SET_END
