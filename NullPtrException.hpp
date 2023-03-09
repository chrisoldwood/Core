////////////////////////////////////////////////////////////////////////////////
//! \file   NullPtrException.hpp
//! \brief  The NullPtrException class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_NULLPTREXCEPTION_HPP
#define CORE_NULLPTREXCEPTION_HPP

#include "Exception.hpp"

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The type of exception thrown when an attempt is made to dereference a null
//! pointer. It is preferable for this to be thrown than the pointer be
//! dereferenced and an Access Violation occur as Structured Exception handling
//! may not be enabled on the thread at the time.

class NullPtrException : public Exception
{
public:
	//! Default constructor.
	NullPtrException();

	//! Construction from a string containing the details.
	NullPtrException(const tstring& details);

	//! Destructor.
	virtual ~NullPtrException() CORE_NO_THROW;
};

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

inline NullPtrException::NullPtrException()
	: Exception(TXT("A null pointer was dereferenced"))
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string containing the details.

inline NullPtrException::NullPtrException(const tstring& details)
	: Exception(details)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

inline NullPtrException::~NullPtrException() CORE_NO_THROW
{
}

//namespace Core
}

#endif // CORE_NULLPTREXCEPTION_HPP
