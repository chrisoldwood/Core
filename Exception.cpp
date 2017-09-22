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
	: m_details()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string containing the details.

Exception::Exception(const tstring& details)
	: m_details(details)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Copy constructor.

Exception::Exception(const Exception& rhs)
	: exception(rhs)
    , m_details(rhs.m_details)
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
		details = m_details.c_str();
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
		details = m_details.c_str();
#else
		// Generate ANSI string on demand.
		if (m_ansiDetails.empty())
			m_ansiDetails = Core::WideToAnsi(m_details);

		details = m_ansiDetails.c_str();
#endif
	}
	catch (...)
	{ }

	return details;
}

//namespace Core
}
