////////////////////////////////////////////////////////////////////////////////
//! \file   RuntimeException.hpp
//! \brief  The RuntimeException class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_RUNTIMEEXCEPTION_HPP
#define CORE_RUNTIMEEXCEPTION_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The exception type thrown for unclassified runtime errors.

class RuntimeException : public Core::Exception
{
public:
	//! Construction from a string containing the details.
	RuntimeException(const tstring& details);

	//! Destructor.
	virtual ~RuntimeException() throw();
};

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string containing the details.

inline RuntimeException::RuntimeException(const tstring& details)
	: Exception(details)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline RuntimeException::~RuntimeException() throw()
{
}

//namespace Core
}

#endif // CORE_RUNTIMEEXCEPTION_HPP
