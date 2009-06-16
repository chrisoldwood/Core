////////////////////////////////////////////////////////////////////////////////
//! \file   InvalidArgException.hpp
//! \brief  The InvalidArgException class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_INVALIDARGEXCEPTION_HPP
#define CORE_INVALIDARGEXCEPTION_HPP

#include "Exception.hpp"

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The type of exception thrown when an invalid argument is passed to a
//! function.

class InvalidArgException : public Exception
{
public:
	//! Construction from a string containing the details.
	InvalidArgException(const tstring& strDetails);

	//! Destructor.
	virtual ~InvalidArgException() throw();
};

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string containing the details.

inline InvalidArgException::InvalidArgException(const tstring& strDetails)
	: Exception(strDetails)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline InvalidArgException::~InvalidArgException() throw()
{
}

//namespace Core
}

#endif // CORE_INVALIDARGEXCEPTION_HPP
