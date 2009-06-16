////////////////////////////////////////////////////////////////////////////////
//! \file   Tokeniser.hpp
//! \brief  The Tokeniser class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_TOKENISER_HPP
#define CORE_TOKENISER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! A class to split a string into separate tokens.

class Tokeniser
{
public:
	//
	// Types.
	//

	//! An array of strings.
	typedef std::vector<tstring> Tokens;

	//! The flags that control the tokensiation.
	enum Flags
	{
		NONE		= 0x0000,	//!< Default.
		MERGE_SEPS	= 0x0001,	//!< Merge consecutive separators.
		RETURN_SEPS = 0x0002,	//!< Return separators as tokens.
	};

public:
	//! Construction from a string, separator list and flags.
	Tokeniser(const tstring& string, const tstring& seps, int flags = NONE);

	//! Destructor.
	~Tokeniser();

	//
	// Methods.
	//

	//! Query if we've reached the end.
	bool moreTokens() const;

	//! Get the next token.
	tstring nextToken();

	//
	// Class methods.
	//

	//! Tokenise the string into an array of strings.
	static size_t split(const tstring& string, const tstring& seps, Tokens& tokens, uint flags = NONE);

private:
	//! The token types.
	enum TokenType
	{
		END_TOKEN		= -1,	//!< Reached end of tokens.

		VALUE_TOKEN		=  1,	//!< Parsing value token.
		SEPARATOR_TOKEN =  2,	//!< Parsing separator list token.
	};

	//
	// Members.
	//
	const tstring&			m_string;		//!< The string to tokenise.
	const tstring&			m_seps;			//!< The list of separators.
	uint					m_flags;		//!< The tokenising control flags.
	TokenType				m_nextToken;	//!< The next token type expected.
	tstring::const_iterator	m_iter;			//!< The string iterator.
};

//namespace Core
}

#endif // CORE_TOKENISER_HPP
