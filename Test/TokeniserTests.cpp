////////////////////////////////////////////////////////////////////////////////
//! \file   TokeniserTests.cpp
//! \brief  The unit tests for the Tokeniser class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/Tokeniser.hpp>

TEST_SET(Tokeniser)
{

TEST_CASE("an empty string returns no tokens")
{
	const tstring           string;
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE(tokens.size() == 0);
}
TEST_CASE_END

TEST_CASE("a string with no separators returns a single token")
{
	const tstring           string(TXT("1"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE(tokens.size() == 1);
	TEST_TRUE(tokens[0] == TXT("1"));
}
TEST_CASE_END

TEST_CASE("a string with a single separator and value either side returns two tokens")
{
	const tstring           string(TXT("1,2"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE(tokens.size() == 2);
	TEST_TRUE(tokens[0] == TXT("1"));
	TEST_TRUE(tokens[1] == TXT("2"));
}
TEST_CASE_END

TEST_CASE("a separator by itself returns 2 empty tokens")
{
	const tstring           string(TXT(","));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE(tokens.size() == 2);
	TEST_TRUE(tokens[0].empty());
	TEST_TRUE(tokens[1].empty());
}
TEST_CASE_END

TEST_CASE("consecutive separators are treated as one when merging is enabled")
{
	const tstring           string(TXT("\r\nabc\r\ndef\r\n"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT("\r\n"), tokens, Core::Tokeniser::MERGE_SEPS);

	TEST_TRUE(tokens.size() == 4);
	TEST_TRUE(tokens[0].empty());
	TEST_TRUE(tokens[1] == TXT("abc"));
	TEST_TRUE(tokens[2] == TXT("def"));
	TEST_TRUE(tokens[3].empty());
}
TEST_CASE_END

TEST_CASE("separators are also returned as tokens when enabled")
{
	const tstring           string(TXT("\r\n"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT("\n\r"), tokens, Core::Tokeniser::RETURN_SEPS);

	TEST_TRUE(tokens.size() == 5);
	TEST_TRUE(tokens[0].empty());
	TEST_TRUE(tokens[1] == TXT("\r"));
	TEST_TRUE(tokens[2].empty());
	TEST_TRUE(tokens[3] == TXT("\n"));
	TEST_TRUE(tokens[4].empty());
}
TEST_CASE_END

TEST_CASE("separators can be both merged and returned as tokens")
{
	const tstring           string(TXT("\r\n"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT("\n\r"), tokens, Core::Tokeniser::RETURN_SEPS|Core::Tokeniser::MERGE_SEPS);

	TEST_TRUE(tokens.size() == 3);
	TEST_TRUE(tokens[0].empty());
	TEST_TRUE(tokens[1] == TXT("\r\n"));
	TEST_TRUE(tokens[2].empty());
}
TEST_CASE_END

TEST_CASE("requesting another token when at the sequence end throws an exception")
{
	Core::Tokeniser tokeniser(TXT(""), TXT(","), Core::Tokeniser::NONE);

	while (tokeniser.moreTokens());

	TEST_THROWS(tokeniser.nextToken());
}
TEST_CASE_END

}
TEST_SET_END
