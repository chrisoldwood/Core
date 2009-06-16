////////////////////////////////////////////////////////////////////////////////
//! \file   ParseException.hpp
//! \brief  The ParseException class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_PARSEEXCEPTION_HPP
#define CORE_PARSEEXCEPTION_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The type of exception thrown for parsing errors.

class ParseException : public Core::Exception
{
public:
	//! Construction from a string containing the details.
	ParseException(const tstring& strDetails);

	//! Destructor.
	virtual ~ParseException() throw();
};

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string containing the details.

inline ParseException::ParseException(const tstring& strDetails)
	: Exception(strDetails)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline ParseException::~ParseException() throw()
{
}

//namespace Core
}

#endif // CORE_PARSEEXCEPTION_HPP
