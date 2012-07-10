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

CmdLineParser::CmdLineParser(SwitchCIter firstSwitch, SwitchCIter lastSwitch)
	: m_firstSwitch(firstSwitch)
	, m_lastSwitch(lastSwitch)
	, m_namedArgs()
	, m_unnamedArgs()
{
	validateSwitches(m_firstSwitch, m_lastSwitch);
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
	tstring	m_seps;

	//! Constructor.
	IsSeparator(const tstring& seps)
		: m_seps(seps)
	{
	}

	//! Compare the value to the switch short name.
	result_type operator()(argument_type character)
	{
		return (tstrchr(m_seps.c_str(), character) != nullptr);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! Parse the command line.

void CmdLineParser::parse(int argc, tchar* argv[], int flags)
{
	reset();

	// Create iterators for the range.
	tchar** currArg = argv;
	tchar** lastArg = currArg+argc;

	// First argument should be the program name.
	if (currArg == lastArg)
		throw CmdLineException(TXT("The first argument must be the process name"));

	++currArg;

	// Create a set of Prefix and Name/Value separator chars.
	tstring prefixes, valueSeps;

	if (flags & ALLOW_UNIX_FORMAT)
	{
		prefixes  += TXT('/');
		valueSeps += TXT('=');
	}

	if (flags & ALLOW_WIN_FORMAT)
	{
		prefixes  += TXT('-');
		valueSeps += TXT(':');
	}

	// For all arguments.
	while (currArg != lastArg)
	{
		const tchar* argument = *currArg;

		// Is a switch?
		if ( ((argument[0] == TXT('-')) && (flags & ALLOW_UNIX_FORMAT))
		  || ((argument[0] == TXT('/')) && (flags & ALLOW_WIN_FORMAT)) )
		{
			// Create iterators for the switch name.
			size_t    length    = tstrlen(argument);
			CharCIter nameFirst = argument;
			CharCIter nameLast  = argument+length;

			// Create iterators for the switch value, if present.
			CharCIter valueFirst = std::find_if(nameFirst, nameLast, IsSeparator(valueSeps));
			CharCIter valueLast  = nameLast;

			// If inline value present, adjust switch name.
			if (valueFirst != valueLast)
			{
				nameLast = valueFirst;
				++valueFirst;
			};

			// Find the switch definition.
			SwitchCIter theSwitch = findSwitch(nameFirst, nameLast);

			if (theSwitch == m_lastSwitch)
				throw CmdLineException(fmt(TXT("Invalid command line switch '%s'"), argument));

			// Check if already specified.
			NamedArgs::iterator namedArg = m_namedArgs.find(theSwitch->m_id);

			if ( (theSwitch->m_occurences == CmdLineSwitch::ONCE) && (namedArg != m_namedArgs.end()) )
				throw CmdLineException(fmt(TXT("The command line switch '%s' can only be specified once"), tstring(nameFirst, nameLast).c_str()));

			// Add to named collection.
			if (namedArg == m_namedArgs.end())
				namedArg = m_namedArgs.insert(std::make_pair(theSwitch->m_id, StringVector())).first;

			// Expecting one or more values?
			if (theSwitch->m_parameters != CmdLineSwitch::NONE)
			{
				// Value is inline?
				if (nameLast != valueFirst)
				{
					namedArg->second.push_back(tstring(valueFirst, valueLast));
				}
				// Value is next argument.
				else
				{
					++currArg;

					if (currArg == lastArg)
						throw CmdLineException(fmt(TXT("The command line switch '%s' requires a value"), tstring(nameFirst, nameLast).c_str()));

					namedArg->second.push_back(*currArg);
				}

				// Potentially more values?
				if (theSwitch->m_parameters == CmdLineSwitch::MULTIPLE)
				{
					++currArg;

					// Until we reach the end of the list OR we hit another switch...
					while ( (currArg != lastArg)
						 && (std::find(prefixes.begin(), prefixes.end(), *currArg[0]) == prefixes.end()) )
					{
						namedArg->second.push_back(*currArg);
						++currArg;
					}

					// Need to rewind to before the last value.
					--currArg;
				}
			}
			// Not expecting one, but found one?
			else if (nameLast != valueFirst)
			{
				throw CmdLineException(fmt(TXT("The command line switch '%s' should not contain a value"), tstring(nameFirst, nameLast).c_str()));
			}

			++currArg;
		}
		// Is an unnamed argument.
		else
		{
			if ((flags & ALLOW_UNNAMED) == 0)
				throw CmdLineException(fmt(TXT("Invalid command line switch '%s'"), argument));

			// Add to unnamed collection.
			m_unnamedArgs.push_back(argument);

			++currArg;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//! Check if a switch was provided.

bool CmdLineParser::isSwitchSet(int id) const
{
	return (m_namedArgs.find(id) != m_namedArgs.end());
}

////////////////////////////////////////////////////////////////////////////////
//! Get the value for a switch. This only returns the first value that was
//! parsed. To obtain the full list iterate the NamedArgs collection directly.

tstring CmdLineParser::getSwitchValue(int id) const
{
	tstring value;

	NamedArgs::const_iterator it = m_namedArgs.find(id);

	if ((it != m_namedArgs.end()) && (!it->second.empty()))
		value = it->second[0];

	return value;
}

////////////////////////////////////////////////////////////////////////////////
//! Generate the list of switches for a usage message.

tstring CmdLineParser::formatSwitches(Format format) const
{
	tstring shortPrefix = (format == WINDOWS) ? TXT("/") : TXT("-");
	tstring longPrefix  = (format == WINDOWS) ? TXT("/") : TXT("--");

	size_t maxShortSwitchLen = 0;
	size_t includeSeparator  = false;

	// Calculate the max length of the short switch name.
	for (SwitchCIter it = m_firstSwitch; it != m_lastSwitch; ++it)
	{
		// Treat switches with no description as internal.
		if (it->m_description == nullptr)
			continue;

		size_t switchLen      = 0;
		size_t shortSwitchLen = 0;

		if (it->m_shortName != nullptr)
		{
			shortSwitchLen = tstrlen(it->m_shortName) + shortPrefix.length() + 1; // "-%s "
			switchLen     += shortSwitchLen;
		}

		if ( (it->m_shortName != nullptr) && (it->m_longName != nullptr) )
		{
			includeSeparator = true; // "| "
		}

		maxShortSwitchLen = std::max(shortSwitchLen, maxShortSwitchLen);
	}

	size_t maxSwitchLen     = maxShortSwitchLen;
	size_t maxLongSwitchLen = 0;

	// Calculate the max length of the long switch name & parameters.
	for (SwitchCIter it = m_firstSwitch; it != m_lastSwitch; ++it)
	{
		// Treat switches with no description as internal.
		if (it->m_description == nullptr)
			continue;

		size_t longSwitchLen = 0;
		size_t switchLen     = maxShortSwitchLen + ((includeSeparator) ? 2 : 0);

		if (it->m_longName != nullptr)
		{
			longSwitchLen = tstrlen(it->m_longName) + longPrefix.length() + 1; // "--%s "
			switchLen     += longSwitchLen;
		}

		if ( (it->m_parameters == CmdLineSwitch::SINGLE) || (it->m_parameters == CmdLineSwitch::MULTIPLE) )
			switchLen += tstrlen(it->m_paramDesc) + 3; // "<%s> "

		if (it->m_parameters == CmdLineSwitch::MULTIPLE)
			switchLen += 4; // "... "

		maxLongSwitchLen   = std::max(longSwitchLen,  maxLongSwitchLen );
		maxSwitchLen       = std::max(switchLen,      maxSwitchLen     );
	}

	tstring usage;

	// Format each line...
	for (SwitchCIter it = m_firstSwitch; it != m_lastSwitch; ++it)
	{
		// Treat switches with no description as internal.
		if (it->m_description == nullptr)
			continue;

		tstring line;

		// Format the switch and value name.
		if (it->m_shortName != nullptr)
			line += fmt(TXT("%s%s "), shortPrefix.c_str(), it->m_shortName);

		if (line.length() < maxShortSwitchLen)
			line += tstring(maxShortSwitchLen-line.length(), TXT(' '));

		if ( (it->m_shortName != nullptr) && (it->m_longName != nullptr) )
			line += TXT("| ");
		else if (includeSeparator)
			line += TXT("  ");

		if (it->m_longName != nullptr)
			line += fmt(TXT("%s%s "), longPrefix.c_str(), it->m_longName);

		if ( (it->m_parameters == CmdLineSwitch::SINGLE) || (it->m_parameters == CmdLineSwitch::MULTIPLE) )
			line += fmt(TXT("<%s> "), it->m_paramDesc);

		if (it->m_parameters == CmdLineSwitch::MULTIPLE)
			line += TXT("... ");

		if (line.length() < maxSwitchLen)
			line += tstring(maxSwitchLen-line.length(), TXT(' '));

		// Append the description.
		if (it->m_description != nullptr)
			line += fmt(TXT(" %s\n"), it->m_description);

		usage += line;
	}

	return usage;
}

////////////////////////////////////////////////////////////////////////////////
//! Reset the internal state.

void CmdLineParser::reset()
{
	m_namedArgs.clear();
	m_unnamedArgs.clear();
}

////////////////////////////////////////////////////////////////////////////////
//! The predicate for matching the switches short name.

struct CmdLineParser::IsShortName : std::unary_function<const CmdLineSwitch&, bool>
{
	//
	// Members.
	//
	CharCIter	m_nameFirst;
	CharCIter	m_nameLast;
	size_t		m_length;

	//! Constructor.
	IsShortName(CharCIter nameFirst, CharCIter nameLast)
		: m_nameFirst(nameFirst), m_nameLast(nameLast)
		, m_length(std::distance(nameFirst, nameLast))
	{
	}

	//! Compare the value to the switch short name.
	result_type operator()(argument_type cmdLineSwitch)
	{
		if (cmdLineSwitch.m_shortName == nullptr)
			return false;

		size_t switchLength = tstrlen(cmdLineSwitch.m_shortName);

		if (m_length != switchLength)
			return false;

//		return std::lexicographical_compare_3way();
		return (tstrnicmp(m_nameFirst, cmdLineSwitch.m_shortName, m_length) == 0);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! The predicate for matching the switches long name.

struct CmdLineParser::IsLongName : std::unary_function<const CmdLineSwitch&, bool>
{
	//
	// Members.
	//
	CharCIter	m_nameFirst;
	CharCIter	m_nameLast;
	size_t		m_length;

	//! Constructor.
	IsLongName(CharCIter nameFirst, CharCIter nameLast)
		: m_nameFirst(nameFirst), m_nameLast(nameLast)
		, m_length(std::distance(nameFirst, nameLast))
	{
	}

	//! Compare the value to the switch long name.
	result_type operator()(argument_type cmdLineSwitch)
	{
		if (cmdLineSwitch.m_longName == nullptr)
			return false;

		size_t switchLength = tstrlen(cmdLineSwitch.m_longName);

		if (m_length != switchLength)
			return false;

//		return std::lexicographical_compare_3way();
		return (tstrnicmp(m_nameFirst, cmdLineSwitch.m_longName, m_length) == 0);
	}
};

////////////////////////////////////////////////////////////////////////////////
//! Try and match the argument to a switch definition.

CmdLineParser::SwitchCIter CmdLineParser::findSwitch(CharCIter nameFirst, CharCIter nameLast)
{
	size_t numChars = std::distance(nameFirst, nameLast);

	ASSERT(numChars > 0);

	// Match the short name? eg /h or -h
	if ( (numChars >= 2) && ( (*nameFirst == TXT('/'))
		|| ((*nameFirst == TXT('-')) && (*(nameFirst+1) != TXT('-'))) ) )
	{
		size_t offset = 1;

		SwitchCIter itMatch = std::find_if(m_firstSwitch, m_lastSwitch, IsShortName(nameFirst+offset, nameLast));

		if (itMatch != m_lastSwitch)
			return itMatch;
	}

	// Match the long name? eg /help or --help
	if ( (numChars >= 3) && ( (*nameFirst == TXT('/'))
		|| ((*nameFirst == TXT('-')) && (*(nameFirst+1) == TXT('-'))) ) )
	{
		size_t offset = (*nameFirst == TXT('/')) ? 1 : 2;

		SwitchCIter itMatch = std::find_if(m_firstSwitch, m_lastSwitch, IsLongName(nameFirst+offset, nameLast));

		if (itMatch != m_lastSwitch)
			return itMatch;
	}

	return m_lastSwitch;
}

////////////////////////////////////////////////////////////////////////////////
//! Validate the switch definitions.

void CmdLineParser::validateSwitches(SwitchCIter first, SwitchCIter last)
{
	std::set<tstring> shortNames, longNames;

	for (SwitchCIter it = first; it != last; ++it)
	{
		if ( (it->m_shortName == nullptr) && (it->m_longName == nullptr) )
			throw BadLogicException(TXT("Neither short nor long switch name provided"));

		if ( (it->m_parameters != CmdLineSwitch::NONE) && (it->m_paramDesc == nullptr) )
			throw BadLogicException(TXT("Parameter expected but no parameter name provided"));

		if (it->m_shortName != nullptr)
		{
			if (shortNames.find(it->m_shortName) != shortNames.end())
				throw BadLogicException(TXT("Duplicate short switch name specified"));

			shortNames.insert(it->m_shortName);
		}

		if (it->m_longName != nullptr)
		{
			if (longNames.find(it->m_longName) != longNames.end())
				throw BadLogicException(TXT("Duplicate long switch name specified"));

			longNames.insert(it->m_longName);
		}
	}
}

//namespace Core
}
