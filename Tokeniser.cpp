////////////////////////////////////////////////////////////////////////////////
//! \file   Tokeniser.cpp
//! \brief  The Tokeniser class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Tokeniser.hpp"
#include "BadLogicException.hpp"

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string, separator list and flags.

Tokeniser::Tokeniser(const tstring& string, const tstring& seps, int flags)
	: m_string(string)
	, m_seps(seps)
	, m_flags(flags)
	, m_nextToken(END_TOKEN)
	, m_iter(m_string.begin())
{
	if (m_iter != m_string.end())
		m_nextToken = VALUE_TOKEN;
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Tokeniser::~Tokeniser()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Query if we've reached the end.

bool Tokeniser::moreTokens() const
{
	return (m_nextToken != END_TOKEN);
}

////////////////////////////////////////////////////////////////////////////////
//! Get the next token.

tstring Tokeniser::nextToken()
{
	tstring::const_iterator start = m_iter;
	tstring::const_iterator end   = start;

	// Next token is a value?
	if (m_nextToken == VALUE_TOKEN)
	{
		// Find next separator or EOS.
		while ( (m_iter != m_string.end()) && (m_seps.find_first_of(*m_iter) == npos) )
			++m_iter;

		end = m_iter;

		// Stopped on a separator?
		if (m_iter != m_string.end())
		{
			// Switch state, if returning separators.
			if (m_flags & RETURN_SEPS)
			{
				m_nextToken = SEPARATOR_TOKEN;
			}
			// Skip separators.
			else 
			{
				++m_iter;

				// Merge consecutive separators?
				if (m_flags & MERGE_SEPS)
				{
					while ( (m_iter != m_string.end()) && (m_seps.find_first_of(*m_iter) != npos) )
						++m_iter;
				}
			}
		}
		// Reached EOS.
		else
		{
			m_nextToken = END_TOKEN;
		}
	}
	// Next token is a separator?
	else if (m_nextToken == SEPARATOR_TOKEN)
	{
		ASSERT(m_seps.find_first_of(*m_iter) != npos);

		++m_iter;

		// Merge consecutive separators?
		if (m_flags & MERGE_SEPS)
		{
			while ( (m_iter != m_string.end()) && (m_seps.find_first_of(*m_iter) != npos) )
				++m_iter;
		}

		end = m_iter;

		// Switch state back to normal.
		m_nextToken = VALUE_TOKEN;
	}
	// Already reached the end.
	else
	{
		throw BadLogicException(TXT("Attempted to iterate past the end of a Tokeniser"));
	}

	return tstring(start, end);
}

////////////////////////////////////////////////////////////////////////////////
//! Tokenise the string into an array of strings.

size_t Tokeniser::split(const tstring& string, const tstring& seps, Tokens& tokens, uint flags)
{
	Tokeniser tokeniser(string, seps, flags);

	while (tokeniser.moreTokens())
		tokens.push_back(tokeniser.nextToken());

	return tokens.size();
}

//namespace Core
}
