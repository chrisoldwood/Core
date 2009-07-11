////////////////////////////////////////////////////////////////////////////////
//! \file   Exception.hpp
//! \brief  The Exception class declaration.
//! \author Chris Oldwood

// Check for previous inclusion
#ifndef CORE_EXCEPTION_HPP
#define CORE_EXCEPTION_HPP

#if _MSC_VER > 1000
#pragma once
#endif

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! The base class for all exception types. This class allows the error message
//! to be ANSI or Unicode, unlike std::exception which is ANSI only. The ctor
//! is protected as you should be throwing a specific derived exception type.

class Exception : public std::exception
{
public:
	//! Destructor.
	virtual ~Exception() throw();

	//
	// Methods.
	//

	//! Get the exception details.
	virtual const tchar* twhat() const throw();

protected:
	//! Default constructor.
	Exception();

	//! Construction from a string containg the details.
	Exception(const tstring& strDetails);

	//! Copy constructor.
	Exception(const Exception& rhs);

	//
	// Members.
	//
	tstring				m_strDetails;		//!< The error message.
#ifdef UNICODE_BUILD
	mutable std::string	m_strAnsiDetails;	//!< The ANSI error message.
#endif

private:
	//! Get the ANSI only exception details.
	virtual const char* what() const throw();
};

//namespace Core
}

#endif // CORE_EXCEPTION_HPP
