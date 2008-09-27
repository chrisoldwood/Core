////////////////////////////////////////////////////////////////////////////////
//! \file   CmdLineException.hpp
//! \brief  The CmdLineException class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_CMDLINEEXCEPTION_HPP
#define CORE_CMDLINEEXCEPTION_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The type of exception thrown for errors parsing the command line.

class CmdLineException : public Exception
{
public:
	//! Construction from a string containing the details.
	CmdLineException(const tstring& strDetails);

	//! Destructor.
	virtual ~CmdLineException();
	
private:
	//
	// Members.
	//
};

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string containing the details.

inline CmdLineException::CmdLineException(const tstring& strDetails)
	: Exception(strDetails)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline CmdLineException::~CmdLineException()
{
}

//namespace Core
}

#endif // CORE_CMDLINEEXCEPTION_HPP
