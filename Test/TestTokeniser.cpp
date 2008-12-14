////////////////////////////////////////////////////////////////////////////////
//! \file   TestTokeniser.cpp
//! \brief  The unit tests for the Tokeniser class.
//! \author Chris Oldwood

#include "stdafx.h"
#include <Core/UnitTest.hpp>
#include <Core/Tokeniser.hpp>

////////////////////////////////////////////////////////////////////////////////
//! The unit tests for the Tokeniser class.

void TestTokeniser()
{
{
	const tstring           string;
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE(tokens.size() == 0);
}
{
	const tstring           string(TXT("1"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE((tokens.size() == 1) && (tokens[0] == TXT("1")));
}
{
	const tstring           string(TXT("1,2"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE((tokens.size() == 2) && (tokens[0] == TXT("1")) && (tokens[1] == TXT("2")));
}
{
	const tstring           string(TXT(","));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT(","), tokens/*, Core::Tokeniser::NONE*/);

	TEST_TRUE((tokens.size() == 2) && (tokens[0].empty()) && (tokens[1].empty()));
}
{
	const tstring           string(TXT("\r\nabc\r\ndef\r\n"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT("\r\n"), tokens, Core::Tokeniser::MERGE_SEPS);

	TEST_TRUE((tokens.size() == 4) && (tokens[0].empty()) && (tokens[1] == TXT("abc")) && (tokens[2] == TXT("def")) && (tokens[3].empty()));
}
{
	const tstring           string(TXT("\r\n"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT("\n\r"), tokens, Core::Tokeniser::RETURN_SEPS);

	TEST_TRUE((tokens.size() == 5) && (tokens[1] == TXT("\r")) && (tokens[3] == TXT("\n")));
}
{
	const tstring           string(TXT("\r\n"));
	Core::Tokeniser::Tokens tokens;

	Core::Tokeniser::split(string, TXT("\n\r"), tokens, Core::Tokeniser::RETURN_SEPS|Core::Tokeniser::MERGE_SEPS);

	TEST_TRUE((tokens.size() == 3) && (tokens[1] == TXT("\r\n")));
}
}
