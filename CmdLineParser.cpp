////////////////////////////////////////////////////////////////////////////////
//! \file   CmdLineParser.cpp
//! \brief  The CmdLineParser class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "CmdLineParser.hpp"
#include "CmdLineException.hpp"
#include "StringUtils.hpp"

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

CmdLineParser::CmdLineParser(SwitchCIter itFirstSwitch, SwitchCIter itLastSwitch)
	: m_itFirstSwitch(itFirstSwitch)
	, m_itLastSwitch(itLastSwitch)
{
#ifdef _DEBUG
	// Verify switch defintions.
	for (SwitchCIter it = m_itFirstSwitch; it != m_itLastSwitch; ++it)
	{
		ASSERT((it->m_pszShortName != nullptr) || (it->m_pszLongName != nullptr));
		ASSERT((it->m_eParameters == CmdLineSwitch::NONE) || (it->m_pszParamDesc != nullptr));
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

CmdLineParser::~CmdLineParser()
{
}

////////////////////////////////////////////////////////////////////////////////
//! The predicate for finding the name/value separator.

struct CmdLineParser::IsSeparator : public std::unary_function<tchar, bool>
{
	//
	// Members.
	//
	tstring	m_strSeps;

	//! Constructor.
	IsSeparator(const tstring& strSeps)
		: m_strSeps(strSeps)
	{
	}

	//! Compare the value to the switch short name.
	result_type operator()(argument_type cChar)
	{
		return (tstrchr(m_strSeps.c_str(), cChar) != nullptr);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! Parse the command line.

void CmdLineParser::Parse(int argc, tchar* argv[], int nFlags)
{
	Reset();

	// Create iterators for the range.
	tchar** itArg     = argv;
	tchar** itLastArg = itArg+argc;

	// First argument should be the program name.
	if (itArg == itLastArg)
		throw CmdLineException(TXT("The first argument must be the process name"));

	++itArg;

	// Create a set of Prefix and Name/Value separator chars.
	tstring strPrefixes, strValSeps;

	if (nFlags & ALLOW_UNIX_FORMAT)
	{
		strPrefixes += TXT('/');
		strValSeps  += TXT('=');
	}

	if (nFlags & ALLOW_WIN_FORMAT)
	{
		strPrefixes += TXT('-');
		strValSeps  += TXT(':');
	}

	// For all arguments.
	while (itArg != itLastArg)
	{
		const tchar* pszArg = *itArg;

		// Is a switch?
		if ( ((pszArg[0] == TXT('-')) && (nFlags & ALLOW_UNIX_FORMAT))
		  || ((pszArg[0] == TXT('/')) && (nFlags & ALLOW_WIN_FORMAT)) )
		{
			// Create iterators for the switch name.
			size_t    nLength     = tstrlen(pszArg);
			CharCIter itNameFirst = pszArg;
			CharCIter itNameLast  = pszArg+nLength;

			// Create iterators for the switch value, if present.
			CharCIter itValueFirst = std::find_if(itNameFirst, itNameLast, IsSeparator(strValSeps));
			CharCIter itValueLast  = itNameLast;

			// If inline value present, adjust switch name.
			if (itValueFirst != itValueLast)
			{
				itNameLast = itValueFirst;
				++itValueFirst;
			};

			// Find the switch definition.
			SwitchCIter itSwitch = FindSwitch(itNameFirst, itNameLast);

			if (itSwitch == m_itLastSwitch)
				throw CmdLineException(Fmt(TXT("Invalid command line switch '%s'"), pszArg));

			// Check if already specified.
			NamedArgs::iterator itNamedArg = m_mapNamedArgs.find(itSwitch->m_nID);

			if ( (itSwitch->m_eOccurences == CmdLineSwitch::ONCE) && (itNamedArg != m_mapNamedArgs.end()) )
				throw CmdLineException(Fmt(TXT("The command line switch '%s' can only be specified once"), tstring(itNameFirst, itNameLast).c_str()));

			// Add to named collection.
			if (itNamedArg == m_mapNamedArgs.end())
				itNamedArg = m_mapNamedArgs.insert(std::make_pair(itSwitch->m_nID, StringVector())).first;

			// Expecting one or more values?
			if (itSwitch->m_eParameters != CmdLineSwitch::NONE)
			{
				// Value is inline?
				if (itNameLast != itValueFirst)
				{
					itNamedArg->second.push_back(tstring(itValueFirst, itValueLast));
				}
				// Value is next argument.
				else
				{
					++itArg;

					if (itArg == itLastArg)
						throw CmdLineException(Fmt(TXT("The command line switch '%s' requires a value"), tstring(itNameFirst, itNameLast).c_str()));

					itNamedArg->second.push_back(*itArg);
				}

				// Potentially more values?
				if (itSwitch->m_eParameters == CmdLineSwitch::MULTIPLE)
				{
					++itArg;

					// Until we reach the end of the list OR we hit another switch...
					while ( (itArg != itLastArg)
						 && (std::find(strPrefixes.begin(), strPrefixes.end(), *itArg[0]) == strPrefixes.end()) )
					{
						itNamedArg->second.push_back(*itArg);
						++itArg;
					}

					// Need to rewind to before the last value.
					itArg -= 2;
				}
			}
			// Not expecting one, but found one?
			else if (itNameLast != itValueFirst)
			{
				throw CmdLineException(Fmt(TXT("The command line switch '%s' should not contain a value"), tstring(itNameFirst, itNameLast).c_str()));
			}

			++itArg;
		}
		// Is an unnamed argument.
		else
		{
			if ((nFlags & ALLOW_UNNAMED) == 0)
				throw CmdLineException(Fmt(TXT("Invalid command line switch '%s'"), pszArg));

			// Add to unnamed collection.
			m_vecUnnamedArgs.push_back(pszArg);

			++itArg;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Check if a switch was provided.

bool CmdLineParser::IsSwitchSet(int nID) const
{
	return (m_mapNamedArgs.find(nID) != m_mapNamedArgs.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Get the value for a switch. This only returns the first value that was
//! parsed. To obtain the full list iterate the NamedArgs collection directly.

tstring CmdLineParser::GetSwitchValue(int nID) const
{
	tstring strValue;

	NamedArgs::const_iterator it = m_mapNamedArgs.find(nID);

	if ((it != m_mapNamedArgs.end()) && (!it->second.empty()))
		strValue = it->second[0];

	return strValue;
}

////////////////////////////////////////////////////////////////////////////////
//! Generate the list of switches for a usage message.

tstring CmdLineParser::FormatSwitches(Format eFormat) const
{
	size_t nMaxSwitchLen   = 0;
	tstring strShortPrefix = (eFormat == WINDOWS) ? TXT("/") : TXT("-");
	tstring strLongPrefix  = (eFormat == WINDOWS) ? TXT("/") : TXT("--");

	// Calculate the max length of the text to the left of the switch descriptions.
	for (SwitchCIter it = m_itFirstSwitch; it != m_itLastSwitch; ++it)
	{
		// Treat switches with no description as internal.
		if (it->m_pszDescription == nullptr)
			continue;

		size_t nSwitchLen = 0;

		if (it->m_pszShortName != nullptr)
			nSwitchLen += tstrlen(it->m_pszShortName) + strShortPrefix.length() + 1; // "-%s "

		if ( (it->m_pszShortName != nullptr) && (it->m_pszLongName != nullptr) )
			nSwitchLen += 2; // "| "

		if (it->m_pszLongName != nullptr)
			nSwitchLen += tstrlen(it->m_pszLongName) + strLongPrefix.length() + 1; // "--%s "

		if ( (it->m_eParameters == CmdLineSwitch::SINGLE) || (it->m_eParameters == CmdLineSwitch::MULTIPLE) )
			nSwitchLen += tstrlen(it->m_pszParamDesc) + 3; // "<%s> "

		if (it->m_eParameters == CmdLineSwitch::MULTIPLE)
			nSwitchLen += 4; // "... "

		nMaxSwitchLen = std::max(nSwitchLen, nMaxSwitchLen);
	}

	tstring strUsage;

	// Format each line...
	for (SwitchCIter it = m_itFirstSwitch; it != m_itLastSwitch; ++it)
	{
		// Treat switches with no description as internal.
		if (it->m_pszDescription == nullptr)
			continue;

		tstring strLine;

		// Format the switch and value name.
		if (it->m_pszShortName != nullptr)
			strLine += Fmt(TXT("%s%s "), strShortPrefix.c_str(), it->m_pszShortName);

		if ( (it->m_pszShortName != nullptr) && (it->m_pszLongName != nullptr) )
			strLine += TXT("| ");

		if (it->m_pszLongName != nullptr)
			strLine += Fmt(TXT("%s%s "), strLongPrefix.c_str(), it->m_pszLongName);

		if ( (it->m_eParameters == CmdLineSwitch::SINGLE) || (it->m_eParameters == CmdLineSwitch::MULTIPLE) )
			strLine += Fmt(TXT("<%s> "), it->m_pszParamDesc);

		if (it->m_eParameters == CmdLineSwitch::MULTIPLE)
			strLine += TXT("... ");

		// Apply padding, if required.
		if (strLine.length() < nMaxSwitchLen)
			strLine += tstring(nMaxSwitchLen-strLine.length(), TXT(' '));

		// Append the description.
		if (it->m_pszDescription != nullptr)
			strLine += Fmt(TXT(" %s\n"), it->m_pszDescription);

		strUsage += strLine;
	}

	return strUsage;
}

////////////////////////////////////////////////////////////////////////////////
//! Reset the internal state.

void CmdLineParser::Reset()
{
	m_mapNamedArgs.clear();
	m_vecUnnamedArgs.clear();
}

////////////////////////////////////////////////////////////////////////////////
//! The predicate for matching the switches short name.

struct CmdLineParser::IsShortName : std::unary_function<const CmdLineSwitch&, bool>
{
	//
	// Members.
	//
	CharCIter	m_itNameFirst;
	CharCIter	m_itNameLast;

	//! Constructor.
	IsShortName(CharCIter itNameFirst, CharCIter itNameLast)
		: m_itNameFirst(itNameFirst), m_itNameLast(itNameLast)
	{
	}

	//! Compare the value to the switch short name.
	result_type operator()(argument_type oSwitch)
	{
		if (oSwitch.m_pszShortName == nullptr)
			return false;

//		return std::lexicographical_compare_3way();
		return (tstrnicmp(m_itNameFirst, oSwitch.m_pszShortName, tstrlen(oSwitch.m_pszShortName)) == 0);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! The predicate for matching the switches long name.

struct CmdLineParser::IsLongName : std::unary_function<const CmdLineSwitch&, bool>
{
	//
	// Members.
	//
	CharCIter	m_itNameFirst;
	CharCIter	m_itNameLast;

	//! Constructor.
	IsLongName(CharCIter itNameFirst, CharCIter itNameLast)
		: m_itNameFirst(itNameFirst), m_itNameLast(itNameLast)
	{
	}

	//! Compare the value to the switch long name.
	result_type operator()(argument_type oSwitch)
	{
		if (oSwitch.m_pszLongName == nullptr)
			return false;

//		return std::lexicographical_compare_3way();
		return (tstrnicmp(m_itNameFirst, oSwitch.m_pszLongName, m_itNameLast - m_itNameFirst) == 0);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! Try and match the argument to a switch definition.

CmdLineParser::SwitchCIter CmdLineParser::FindSwitch(CharCIter itNameFirst, CharCIter itNameLast)
{
	size_t nChars = std::distance(itNameFirst, itNameLast);

	ASSERT(nChars > 0);

	// Match the short name? eg /h or -h
	if ( (nChars >= 2) && ( (*itNameFirst == TXT('/'))
		|| ((*itNameFirst == TXT('-')) && (*(itNameFirst+1) != TXT('-'))) ) )
	{
		size_t nOffset = 1;

		SwitchCIter itMatch = std::find_if(m_itFirstSwitch, m_itLastSwitch, IsShortName(itNameFirst+nOffset, itNameLast));

		if (itMatch != m_itLastSwitch)
			return itMatch;
	}

	// Match the long name? eg /help or --help
	if ( (nChars >= 3) && ( (*itNameFirst == TXT('/'))
		|| ((*itNameFirst == TXT('-')) && (*(itNameFirst+1) == TXT('-'))) ) )
	{
		size_t nOffset = (*itNameFirst == TXT('/')) ? 1 : 2;

		SwitchCIter itMatch = std::find_if(m_itFirstSwitch, m_itLastSwitch, IsLongName(itNameFirst+nOffset, itNameLast));

		if (itMatch != m_itLastSwitch)
			return itMatch;
	}

	return m_itLastSwitch;
}

//namespace Core
}
