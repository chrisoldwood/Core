////////////////////////////////////////////////////////////////////////////////
//! \file   NotImplException.hpp
//! \brief  The NotImplException class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_NOTIMPLEXCEPTION_HPP
#define CORE_NOTIMPLEXCEPTION_HPP

#include "Exception.hpp"

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The exception type thrown for features that have not been implemented.

class NotImplException : public Exception
{
public:
	//! Construction from a string containing the details.
	NotImplException(const std::tstring& strDetails);

	//! Destructor.
	virtual ~NotImplException();
};

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string containg the details.

inline NotImplException::NotImplException(const std::tstring& strDetails)
	: Exception(strDetails)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline NotImplException::~NotImplException()
{
}

//namespace Core
}

#endif // CORE_NOTIMPLEXCEPTION_HPP
