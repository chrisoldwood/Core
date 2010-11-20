////////////////////////////////////////////////////////////////////////////////
//! \file   Exception.cpp
//! \brief  The Exception class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "Exception.hpp"
#include "AnsiWide.hpp"

namespace Core
{

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

Exception::Exception()
	: m_strDetails()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string containg the details.

Exception::Exception(const tstring& strDetails)
	: m_strDetails(strDetails)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor.

Exception::Exception(const Exception& rhs)
	: m_strDetails(rhs.m_strDetails)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

Exception::~Exception() throw()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Get the exception details.

const tchar* Exception::twhat() const throw()
{
	const tchar* details = TXT("(unknown)");

	try
	{
		details = m_strDetails.c_str();
	}
	catch (...)
	{ }

	return details;
}

////////////////////////////////////////////////////////////////////////////////
//! Get the ANSI only exception details. This method is marked private to avoid
//! it being called by accident instead of the build agnostic method What().
//! However it could still be invoked through the base class, so we still need
//! to generate an ANSI version on demand.

const char* Exception::what() const throw()
{
	const char* details = "(unknown)";

	try
	{
		ASSERT_FALSE();

#ifdef ANSI_BUILD
		details = m_strDetails.c_str();
#else
		// Generate ANSI string on demand.
		if (m_strAnsiDetails.empty())
			m_strAnsiDetails = Core::WideToAnsi(m_strDetails);

		details = m_strAnsiDetails.c_str();
#endif
	}
	catch (...)
	{ }

	return details;
}

//namespace Core
}
