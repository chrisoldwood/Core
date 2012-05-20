////////////////////////////////////////////////////////////////////////////////
//! \file   CmdLineParser.cpp
//! \brief  The CmdLineParser class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "CmdLineParser.hpp"
#include "CmdLineException.hpp"
#include "StringUtils.hpp"
#include <algorithm>
#include "BadLogicException.hpp"
#include <set>

#ifdef __GNUG__
// 'X' has pointer data members [but is copyable]
// base class 'X' has a non-virtual destructor
#pragma GCC diagnostic ignored "-Weffc++"
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

CmdLineParser::CmdLineParser(SwitchCIter itFirstSwitch, SwitchCIter itLastSwitch)
	: m_itFirstSwitch(itFirstSwitch)
	, m_itLastSwitch(itLastSwitch)
	, m_mapNamedArgs()
	, m_vecUnnamedArgs()
{
	validateSwitches(m_itFirstSwitch, m_itLastSwitch);
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

void CmdLineParser::parse(int argc, tchar* argv[], int nFlags)
{
	reset();

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
			SwitchCIter itSwitch = findSwitch(itNameFirst, itNameLast);

			if (itSwitch == m_itLastSwitch)
				throw CmdLineException(fmt(TXT("Invalid command line switch '%s'"), pszArg));

			// Check if already specified.
			NamedArgs::iterator itNamedArg = m_mapNamedArgs.find(itSwitch->m_nID);

			if ( (itSwitch->m_eOccurences == CmdLineSwitch::ONCE) && (itNamedArg != m_mapNamedArgs.end()) )
				throw CmdLineException(fmt(TXT("The command line switch '%s' can only be specified once"), tstring(itNameFirst, itNameLast).c_str()));

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
						throw CmdLineException(fmt(TXT("The command line switch '%s' requires a value"), tstring(itNameFirst, itNameLast).c_str()));

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
					--itArg;
				}
			}
			// Not expecting one, but found one?
			else if (itNameLast != itValueFirst)
			{
				throw CmdLineException(fmt(TXT("The command line switch '%s' should not contain a value"), tstring(itNameFirst, itNameLast).c_str()));
			}

			++itArg;
		}
		// Is an unnamed argument.
		else
		{
			if ((nFlags & ALLOW_UNNAMED) == 0)
				throw CmdLineException(fmt(TXT("Invalid command line switch '%s'"), pszArg));

			// Add to unnamed collection.
			m_vecUnnamedArgs.push_back(pszArg);

			++itArg;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Check if a switch was provided.

bool CmdLineParser::isSwitchSet(int nID) const
{
	return (m_mapNamedArgs.find(nID) != m_mapNamedArgs.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Get the value for a switch. This only returns the first value that was
//! parsed. To obtain the full list iterate the NamedArgs collection directly.

tstring CmdLineParser::getSwitchValue(int nID) const
{
	tstring strValue;

	NamedArgs::const_iterator it = m_mapNamedArgs.find(nID);

	if ((it != m_mapNamedArgs.end()) && (!it->second.empty()))
		strValue = it->second[0];

	return strValue;
}

////////////////////////////////////////////////////////////////////////////////
//! Generate the list of switches for a usage message.

tstring CmdLineParser::formatSwitches(Format eFormat) const
{
	tstring strShortPrefix = (eFormat == WINDOWS) ? TXT("/") : TXT("-");
	tstring strLongPrefix  = (eFormat == WINDOWS) ? TXT("/") : TXT("--");

	size_t maxShortSwitchLen = 0;
	size_t includeSeparator  = false;

	// Calculate the max length of the short switch name.
	for (SwitchCIter it = m_itFirstSwitch; it != m_itLastSwitch; ++it)
	{
		// Treat switches with no description as internal.
		if (it->m_pszDescription == nullptr)
			continue;

		size_t switchLen      = 0;
		size_t shortSwitchLen = 0;

		if (it->m_pszShortName != nullptr)
		{
			shortSwitchLen = tstrlen(it->m_pszShortName) + strShortPrefix.length() + 1; // "-%s "
			switchLen     += shortSwitchLen;
		}

		if ( (it->m_pszShortName != nullptr) && (it->m_pszLongName != nullptr) )
		{
			includeSeparator = true; // "| "
		}

		maxShortSwitchLen = std::max(shortSwitchLen, maxShortSwitchLen);
	}

	size_t maxSwitchLen     = maxShortSwitchLen;
	size_t maxLongSwitchLen = 0;

	// Calculate the max length of the long switch name & parameters.
	for (SwitchCIter it = m_itFirstSwitch; it != m_itLastSwitch; ++it)
	{
		// Treat switches with no description as internal.
		if (it->m_pszDescription == nullptr)
			continue;

		size_t longSwitchLen = 0;
		size_t switchLen     = maxShortSwitchLen + ((includeSeparator) ? 2 : 0);

		if (it->m_pszLongName != nullptr)
		{
			longSwitchLen = tstrlen(it->m_pszLongName) + strLongPrefix.length() + 1; // "--%s "
			switchLen     += longSwitchLen;
		}

		if ( (it->m_eParameters == CmdLineSwitch::SINGLE) || (it->m_eParameters == CmdLineSwitch::MULTIPLE) )
			switchLen += tstrlen(it->m_pszParamDesc) + 3; // "<%s> "

		if (it->m_eParameters == CmdLineSwitch::MULTIPLE)
			switchLen += 4; // "... "

		maxLongSwitchLen   = std::max(longSwitchLen,  maxLongSwitchLen );
		maxSwitchLen       = std::max(switchLen,      maxSwitchLen     );
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
			strLine += fmt(TXT("%s%s "), strShortPrefix.c_str(), it->m_pszShortName);

		if (strLine.length() < maxShortSwitchLen)
			strLine += tstring(maxShortSwitchLen-strLine.length(), TXT(' '));

		if ( (it->m_pszShortName != nullptr) && (it->m_pszLongName != nullptr) )
			strLine += TXT("| ");
		else if (includeSeparator)
			strLine += TXT("  ");

		if (it->m_pszLongName != nullptr)
			strLine += fmt(TXT("%s%s "), strLongPrefix.c_str(), it->m_pszLongName);

		if ( (it->m_eParameters == CmdLineSwitch::SINGLE) || (it->m_eParameters == CmdLineSwitch::MULTIPLE) )
			strLine += fmt(TXT("<%s> "), it->m_pszParamDesc);

		if (it->m_eParameters == CmdLineSwitch::MULTIPLE)
			strLine += TXT("... ");

		if (strLine.length() < maxSwitchLen)
			strLine += tstring(maxSwitchLen-strLine.length(), TXT(' '));

		// Append the description.
		if (it->m_pszDescription != nullptr)
			strLine += fmt(TXT(" %s\n"), it->m_pszDescription);

		strUsage += strLine;
	}

	return strUsage;
}

////////////////////////////////////////////////////////////////////////////////
//! Reset the internal state.

void CmdLineParser::reset()
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
	size_t		m_length;

	//! Constructor.
	IsShortName(CharCIter itNameFirst, CharCIter itNameLast)
		: m_itNameFirst(itNameFirst), m_itNameLast(itNameLast)
		, m_length(std::distance(itNameFirst, itNameLast))
	{
	}

	//! Compare the value to the switch short name.
	result_type operator()(argument_type oSwitch)
	{
		if (oSwitch.m_pszShortName == nullptr)
			return false;

		size_t switchLength = tstrlen(oSwitch.m_pszShortName);

		if (m_length != switchLength)
			return false;

//		return std::lexicographical_compare_3way();
		return (tstrnicmp(m_itNameFirst, oSwitch.m_pszShortName, m_length) == 0);
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
	size_t		m_length;

	//! Constructor.
	IsLongName(CharCIter itNameFirst, CharCIter itNameLast)
		: m_itNameFirst(itNameFirst), m_itNameLast(itNameLast)
		, m_length(std::distance(itNameFirst, itNameLast))
	{
	}

	//! Compare the value to the switch long name.
	result_type operator()(argument_type oSwitch)
	{
		if (oSwitch.m_pszLongName == nullptr)
			return false;

		size_t switchLength = tstrlen(oSwitch.m_pszLongName);

		if (m_length != switchLength)
			return false;

//		return std::lexicographical_compare_3way();
		return (tstrnicmp(m_itNameFirst, oSwitch.m_pszLongName, m_length) == 0);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! Try and match the argument to a switch definition.

CmdLineParser::SwitchCIter CmdLineParser::findSwitch(CharCIter itNameFirst, CharCIter itNameLast)
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

////////////////////////////////////////////////////////////////////////////////
//! Validate the switch definitions.

void CmdLineParser::validateSwitches(SwitchCIter first, SwitchCIter last)
{
	std::set<tstring> shortNames, longNames;

	for (SwitchCIter it = first; it != last; ++it)
	{
		if ( (it->m_pszShortName == nullptr) && (it->m_pszLongName == nullptr) )
			throw BadLogicException(TXT("Neither short nor long switch name provided"));

		if ( (it->m_eParameters != CmdLineSwitch::NONE) && (it->m_pszParamDesc == nullptr) )
			throw BadLogicException(TXT("Parameter expected but no parameter name provided"));

		if (it->m_pszShortName != nullptr)
		{
			if (shortNames.find(it->m_pszShortName) != shortNames.end())
				throw BadLogicException(TXT("Duplicate short switch name specified"));

			shortNames.insert(it->m_pszShortName);
		}

		if (it->m_pszLongName != nullptr)
		{
			if (longNames.find(it->m_pszLongName) != longNames.end())
				throw BadLogicException(TXT("Duplicate long switch name specified"));

			longNames.insert(it->m_pszLongName);
		}
	}
}

//namespace Core
}
