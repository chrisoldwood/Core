////////////////////////////////////////////////////////////////////////////////
//! \file   CmdLineParser.hpp
//! \brief  The CmdLineParser class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_CMDLINEPARSER_HPP
#define CORE_CMDLINEPARSER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include "CmdLineSwitch.hpp"

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! A command line parser. The parser supports both Unix style (eg -h & --help)
//! and Windows style (eg /h & /help) switches. The value for the switch can
//! be specifed after the switch name (eg -p <file>) or inline in either format
//! (eg /p:<file> or --print=<file>). A switch can also be referenced multiple
//! times and/or have multiple values (eg -x <value> -x <value> or -x <value>
//! <value>).

class CmdLineParser
{
public:
	//
	// Types.
	//

	//! An iterator into command line switch table.
	typedef const CmdLineSwitch* SwitchCIter;
	//! A vector of strings.
	typedef std::vector<tstring> StringVector;
	//! The type used to store the named arguments.
	typedef std::map<int, StringVector> NamedArgs;
	//! The type used to store the unnamed arguments.
	typedef StringVector UnnamedArgs;

	//! The switch formats
	enum Format
	{
		WINDOWS			= 0x0001,		//!< Allow Windows format switches.
		UNIX			= 0x0002,		//!< Allow Unix format switches.
	};

	//! The parsing flags.
	enum Flag
	{
		ALLOW_WIN_FORMAT	= WINDOWS,		//!< Allow Windows format switches.
		ALLOW_UNIX_FORMAT	= UNIX,			//!< Allow Unix format switches.
		ALLOW_UNNAMED		= 0x0004,		//!< Allow unnamed arguments.

		ALLOW_ANY_FORMAT	= (ALLOW_WIN_FORMAT | ALLOW_UNIX_FORMAT),
		DEFAULTS			= ALLOW_ANY_FORMAT | ALLOW_UNNAMED,
	};

public:
	//! Default constructor.
	CmdLineParser(SwitchCIter itFirstSwitch, SwitchCIter itLastSwitch);

	//! Destructor.
	~CmdLineParser();
	
	//
	// Properties.
	//

	//! Get the collection of named arguments.
	const NamedArgs& GetNamedArgs() const;

	//! Get the collection of unnamed arguments.
	const UnnamedArgs& GetUnnamedArgs() const;

	//
	// Methods.
	//

	//! Parse the command line.
	void Parse(int argc, tchar* argv[], int nFlags = DEFAULTS); // throw(CmdLineException)

	//! Check if a switch was provided.
	bool IsSwitchSet(int nID) const;

	//! Get the value for a switch.
	tstring GetSwitchValue(int nID) const;

	//! Generate the list of switches for a usage message.
	tstring FormatSwitches(Format eFormat) const;

private:
	//! An iterator for c-style strings.
	typedef const tchar* CharCIter;

	//
	// Members.
	//
	SwitchCIter		m_itFirstSwitch;	//!< The first switch in the table.
	SwitchCIter		m_itLastSwitch;		//!< The last switch in the table.
	NamedArgs		m_mapNamedArgs;		//!< The collection of named arguments.
	UnnamedArgs		m_vecUnnamedArgs;	//!< The collection of unnamed arguments.

	//
	// Internal methods.
	//

	//! Reset the internal state.
	void Reset();

	//! Try and match the argument to a switch definition.
	SwitchCIter FindSwitch(CharCIter itNameFirst, CharCIter itNameLast);

	//
	// Forward declarations.
	//

	//! The predicate for finding the name/value separator.
	struct IsSeparator;
	//! The predicate for matching the switches short name.
	struct IsShortName;
	//! The predicate for matching the switches long name.
	struct IsLongName;
};

////////////////////////////////////////////////////////////////////////////////
//! Get the collection of named arguments.

inline const CmdLineParser::NamedArgs& CmdLineParser::GetNamedArgs() const
{
	return m_mapNamedArgs;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the collection of unnamed arguments.

inline const CmdLineParser::UnnamedArgs& CmdLineParser::GetUnnamedArgs() const
{
	return m_vecUnnamedArgs;
}

//namespace Core
}

#endif // CORE_CMDLINEPARSER_HPP
