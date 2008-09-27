////////////////////////////////////////////////////////////////////////////////
//! \file   CmdLineSwitch.hpp
//! \brief  The CmdLineSwitch structure declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_CMDLINESWITCH_HPP
#define CORE_CMDLINESWITCH_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The definition of a command line switch. The CmdLineParser class uses a
//! table of these to parse a command line. Each switch must have at least a
//! short or long name.

struct CmdLineSwitch
{
	//
	// Types.
	//

	//! The number of occurences.
	enum Occurences
	{
		ONCE,
		MANY,
	};

	//! The number of parameters.
	enum Parameters
	{
		NONE,
		SINGLE,
		MULTIPLE,
	};

	//
	// Members.
	//
	int				m_nID;				//!< The identifier for the switch.
	const tchar*	m_pszShortName;		//!< The switch short name.
	const tchar*	m_pszLongName;		//!< The switch long name.
	Occurences		m_eOccurences;		//!< The number of ocurrences allowed.
	Parameters		m_eParameters;		//!< The number of addtional parameters.
	const tchar*	m_pszParamDesc;		//!< The parameter description.
	const tchar*	m_pszDescription;	//!< The switch description.
};

//namespace Core
}

#endif // CORE_CMDLINESWITCH_HPP
