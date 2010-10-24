////////////////////////////////////////////////////////////////////////////////
//! \file   TestTokeniser.cpp
//! \brief  The unit tests for the Tokeniser class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/Tokeniser.hpp>

TEST_SET(Tokeniser)
{

TEST_CASE(Tokeniser, emptyString)
{
	const tstring           string;
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE(tokens.size() == 0);
}
TEST_CASE_END

TEST_CASE(Tokeniser, singleToken)
{
	const tstring           string(TXT("1"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE((tokens.size() == 1) && (tokens[0] == TXT("1")));
}
TEST_CASE_END

TEST_CASE(Tokeniser, multipleTokens)
{
	const tstring           string(TXT("1,2"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE((tokens.size() == 2) && (tokens[0] == TXT("1")) && (tokens[1] == TXT("2")));
}
TEST_CASE_END

TEST_CASE(Tokeniser, separatorOnly)
{
	const tstring           string(TXT(","));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE((tokens.size() == 2) && (tokens[0].empty()) && (tokens[1].empty()));
}
TEST_CASE_END

TEST_CASE(Tokeniser, mergingSeparators)
{
	const tstring           string(TXT("\r\nabc\r\ndef\r\n"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT("\r\n"), tokens, Core::Tokeniser::MERGE_SEPS);

	TEST_TRUE((tokens.size() == 4) && (tokens[0].empty()) && (tokens[1] == TXT("abc")) && (tokens[2] == TXT("def")) && (tokens[3].empty()));
}
TEST_CASE_END

TEST_CASE(Tokeniser, returnSeparators)
{
	const tstring           string(TXT("\r\n"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT("\n\r"), tokens, Core::Tokeniser::RETURN_SEPS);

	TEST_TRUE((tokens.size() == 5) && (tokens[1] == TXT("\r")) && (tokens[3] == TXT("\n")));
}
TEST_CASE_END

TEST_CASE(Tokeniser, mergeAndReturnSeparators)
{
	const tstring           string(TXT("\r\n"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT("\n\r"), tokens, Core::Tokeniser::RETURN_SEPS|Core::Tokeniser::MERGE_SEPS);

	TEST_TRUE((tokens.size() == 3) && (tokens[1] == TXT("\r\n")));
}
TEST_CASE_END

TEST_CASE(Tokeniser, overstepIteration)
{
	Core::Tokeniser tokeniser(TXT(""), TXT(","), Core::Tokeniser::NONE);

	while (tokeniser.moreTokens());

	TEST_THROWS(tokeniser.nextToken());
}
TEST_CASE_END

}
TEST_SET_END
