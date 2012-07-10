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
	int				m_id;				//!< The identifier for the switch.
	const tchar*	m_shortName;		//!< The switch short name.
	const tchar*	m_longName;			//!< The switch long name.
	Occurences		m_occurences;		//!< The number of ocurrences allowed.
	Parameters		m_parameters;		//!< The number of addtional parameters.
	const tchar*	m_paramDesc;		//!< The parameter description.
	const tchar*	m_description;		//!< The switch description.
};

//namespace Core
}

#endif // CORE_CMDLINESWITCH_HPP
